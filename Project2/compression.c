#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sched.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <zlib.h>

#define BLOCK_SIZE 4096

//#define DEBUGGING

#ifdef DEBUGGING
#define DEBUG_PRINT 1
#else
#define DEBUG_PRINT 0
#endif

// Use to hide debugging print statements when not needed
#define debug_fprintf(file, format_string, ...) \
    do { \
        if(DEBUG_PRINT) fprintf(file, format_string, ##__VA_ARGS__);\
    } while(0);

#define debug_printf(format_string, ...) \
    do { \
        debug_fprintf(stdout, format_string, ##__VA_ARGS__);\
    } while (0);


// Linked list elemnet containing job info for compressing a block of data
// Stored as a linked list since later jobs may complete before earlier jobs
// If a thread completes and no job_ll objects are avaialbale, we allocate a new one
// Once the output data is sent to the file, the job_ll element can be reused
typedef struct job_ll {
    char job_available; // Flag marking if the job instance is available to be reused
    uint8_t *in_data; // Pointer to the data to be compressed
    int in_data_length; // Length of input data
    uint8_t *out_data; // Pointer to the output data
    size_t out_data_allocated; // Size of the allocated output buffer
    size_t out_data_length; // Actual length of the output data
    char in_data_ready; // Flag marking if the input data is ready
    char out_data_ready; // Flag marking if the output data is ready

    size_t block_num; // The BLOCK_SIZE sized block this job is for
    struct job_ll *next; // Pointer to the next job in the linked list
} job_ll;


// Structure used to send arguments to the worker threads
typedef struct worker_args_t {
    job_ll **curr_job; // Pointer to pointer of current job. Pointer of a pointer so that we can update the job
    char *exit; // Indicator when work is done to exit
    char *start; // Indicator when to start the next job
    int worker_num; // Save worker number for debugging/printing
} worker_args;


void *worker_thread(void *args)
{
    worker_args *input = (worker_args *)args;
    debug_printf("Worker %d starting\n", input->worker_num);

    // Keep looping until an exit signal is provided
    while(*(input->exit) == 0)
    {
        // Sleep while the main thread is getting new job
        while (*(input->start) == 0)
        {
            if (*(input->exit)) break;
        }
        (*(input->start)) = 0;

        while ((*(input->curr_job))->in_data_ready == 0)
        {
            if (*(input->exit)) break;
        }

        job_ll *curr_job = (*(input->curr_job));

/*
        // Testing: Just copy the input to the output
        // Used to ensure that the output is created correctly and in the correct order
        size_t output_allocated = ((*input->curr_job))->in_data_length;
        if (((*input->curr_job))->out_data == NULL)
        {
            // No output buffer has been allocated in this job, create a new one
            (*(input->curr_job))->out_data = malloc(output_allocated);
            (*(input->curr_job))->out_data_allocated = output_allocated;
            debug_printf("Allocing %d\n", input->worker_num);
        }
        else if ((*(input->curr_job))->out_data_allocated < output_allocated)
        {
            (*(input->curr_job))->out_data = realloc((*(input->curr_job))->out_data, output_allocated);
            (*(input->curr_job))->out_data_allocated = output_allocated;
            debug_printf("re-Allocing %d\n", input->worker_num);
        }
        (*(input->curr_job))->out_data_length = (*(input->curr_job))->in_data_length;
        memcpy((*(input->curr_job))->out_data, (*(input->curr_job))->in_data, output_allocated);
*/

        // Start compression

        size_t output_allocated = compressBound(curr_job->in_data_length);

        if (curr_job->out_data == NULL)
        {
            // No output buffer allocated in this job, create a new one
            curr_job->out_data = malloc(output_allocated);
            curr_job->out_data_allocated = output_allocated;
            debug_printf("Allocing %d\n", input->worker_num);
        }
        else if (curr_job->out_data_allocated < output_allocated)
        {
            // The current buffer is not big enough, resize it
            curr_job->out_data = realloc(curr_job->out_data, output_allocated);
            curr_job->out_data_allocated = output_allocated;
            debug_printf("re-Allocing %d\n", input->worker_num);
        }

        // Set the length to the size of the allocated buffer
        // compress2 will update this with the actual length of the output data
        size_t temp = curr_job->out_data_allocated;
        int ret = compress2(curr_job->out_data, &temp, curr_job->in_data, curr_job->in_data_length, 9);

        // Check compress status
        if (ret != Z_OK)
        {
            if (ret == Z_BUF_ERROR)
                fprintf(stderr, "Z_BUF_ERROR\n");
            if (ret == Z_MEM_ERROR)
                fprintf(stderr, "Z_MEM_ERROR\n");
            if (ret == Z_STREAM_ERROR)
                fprintf(stderr, "Z_STREAM_ERROR\n");
            exit(1);
        }

        // Store the output length
        curr_job->out_data_length = temp;

        debug_printf("in_data_len: %d\n", curr_job->in_data_length);
        debug_printf("out_data_len: %d\n", curr_job->out_data_length);

        // End Compression

        curr_job->in_data_ready = 0;
        curr_job->out_data_ready = 1;
    }

    debug_printf("Worker %d finished with last job %lu\n", input->worker_num, ((*input->curr_job))->block_num);
}


int worker_available(worker_args *workers, int num_workers)
{
    // Loop through all the worker threads
    for (int i = 0; i < num_workers; i++)
    {
        job_ll **worker_job_ptr = workers[i].curr_job;
        if (worker_job_ptr == NULL)
        {
            // If they don't have a job allocated, return this index
            return i;
        }
        job_ll *worker_job = *worker_job_ptr;
        if (worker_job->in_data_ready == 0)
        {
            // If the job is no longer running, return this index
            return i;
        }
    }
    // No worker threads available
    return -1;
}


// Search through the linked list for the job with the desired block number
job_ll *get_job_with_index(job_ll *head, size_t idx)
{
    while (head != NULL)
    {
        if (head->block_num == idx) return head;
        head = head->next;
    }
    return NULL;
}

// Searched through the linked list for a job not currently being used
// Create a new job if none are available
job_ll *get_free_job(job_ll **head)
{
    if (*head == NULL)
    {
        debug_printf("Creating new head\n");
        *head = calloc(1, sizeof(job_ll));
        (*head)->block_num = -1;
        (*head)->job_available= 0;
        return *head;
    }
    job_ll *curr = *head;
    job_ll *prev;
    while (curr != NULL)
    {
        //if (curr->in_data_ready == 0 && curr->out_data_ready == 0 && curr->block_num == -1)
        if (curr->job_available == 1)
        {
            debug_printf("Returning existing ll element\n");
            curr->job_available = 0;
            curr->block_num = -1;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }
    debug_printf("Adding new element to linked list\n");
    prev->next = calloc(1, sizeof(job_ll));
    prev->next->block_num = -1;
    prev->next->job_available = 0;
    return prev->next;
}

// Structure for sending arguments to the output thread
typedef struct output_thread_args_t
{
    job_ll **jobs_head_ptr;     // Pointer to the start of the linked list of jobs
    size_t *curr_output_block;  // Current block being outputted
    char *exit;                 // Signal from main thread when to exit
    int output_fd;              // File descriptor to send the output data too
} output_thread_args;


// Go through the linked list, output blocks in order
size_t total_bytes_written = 0;
void handle_completion(job_ll *jobs_head, size_t *curr_output_block, int output_fd)
{
    job_ll *job_to_output = NULL;
    // Get the next job on the current block number
    while (job_to_output = get_job_with_index(jobs_head, *curr_output_block))
    {
        if (job_to_output->out_data_ready == 0) break; // Output data hasn't been created yet
        // Write the block to a file
        size_t num_bytes_written = 0;
        //printf("Writing block %ld of size %lu to file\n", *curr_output_block, job_to_output->out_data_length);
        while (num_bytes_written != job_to_output->out_data_length)
        {
            int ret = write(output_fd, job_to_output->out_data, job_to_output->out_data_length);
            if (ret == -1)
            {
                perror("write");
                exit(1);
            }
            num_bytes_written += ret;
        }
        total_bytes_written += num_bytes_written;

        // Mark this block as unused
        job_to_output->out_data_ready = 0;
        //job_to_output->block_num = -1;

        (*(curr_output_block))++; // Increment the current output block number
        job_to_output->job_available = 1;
    }
}

void *output_thread(void *args)
{
    // Keep looking at the output linked list and output to file as blocks are completed
    output_thread_args *input = (output_thread_args *)args;
    debug_printf("Starting output thread\n");
    while (*(input->exit) == 0)
    {
        handle_completion(*(input->jobs_head_ptr), input->curr_output_block, input->output_fd);
        sched_yield();
    }
    debug_printf("Ending output thread\n");
}


void usage(char *fn)
{
    printf("Usage:\n$ %s filename num_threads\n", fn);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(1);
    }
    char *FILENAME = argv[1];

    char *OUTPUT_FILENAME = malloc(strlen(FILENAME) + 4 + 1);
    strcpy(OUTPUT_FILENAME, FILENAME);
    strcat(OUTPUT_FILENAME + strlen(FILENAME), ".out");

    int NUM_THREADS = atoi(argv[2]);

    printf("Running compression on %s with %d threads. Output will be sent to %s\n", FILENAME, NUM_THREADS, OUTPUT_FILENAME);

    size_t curr_input_block = 0;
    size_t curr_output_block = 0;

    // Read the size of the file
    struct stat statbuf;
    int ret = stat(FILENAME, &statbuf);
    if (ret == -1)
    {
        perror("stat");
        exit(1);
    }

    if (NUM_THREADS > statbuf.st_size / BLOCK_SIZE)
    {
        // Don't create extra threads
        NUM_THREADS = statbuf.st_size / BLOCK_SIZE;
    }

    // Open the input and output files
    int fd = open(FILENAME, O_RDONLY);
    int output_fd = open(OUTPUT_FILENAME, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1 || output_fd == -1)
    {
        perror("open");
        exit(1);
    }

    // TODO: Check for mmap errors
    // Map the input file as a memory region. Since file is O_RDONLY, PROT_READ, and MAP_PRIVATEm
    // it will only load into memory segments as they are needed
    uint8_t *data = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    printf("Loaded input file with %ld bytes\n", statbuf.st_size);

    // Allocate arguments and flags for each worker thread
    worker_args *all_worker_args = calloc(NUM_THREADS, sizeof(worker_args));
    pthread_t *threads = malloc(NUM_THREADS * sizeof(pthread_t));
    char *exit_flags = calloc(NUM_THREADS, sizeof(char));
    char *start_flags = calloc(NUM_THREADS, sizeof(char));

    // Create worker threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        all_worker_args[i].exit = exit_flags + i;
        all_worker_args[i].start = start_flags + i;
        all_worker_args[i].curr_job = NULL;
        all_worker_args[i].worker_num = i;
        ret = pthread_create(threads + i, NULL, worker_thread, (void *)(all_worker_args + i));
        if (ret != 0)
        {
            fprintf(stderr, "pthread error\n");
            exit(1);
        }
    };

    size_t num_read = 0;

    job_ll *job_ll_head = NULL;

    // Create output thread to write output data to file
    pthread_t output_thread_handle;
    output_thread_args output_args;
    output_args.jobs_head_ptr = &job_ll_head;
    output_args.curr_output_block = &curr_output_block;
    output_args.exit = malloc(sizeof(char));
    (*output_args.exit) = 0;
    output_args.output_fd = output_fd;
    char *output_exit = output_args.exit;
    ret = pthread_create(&output_thread_handle, NULL, output_thread, (void *)&output_args);
    if (ret != 0)
    {
        fprintf(stderr, "pthread error\n");
        exit(1);
    }

    int percentage = 0; // For progress printing
    // Read through the whole input file
    while (num_read < statbuf.st_size)
    {
        int current_percent = ((float)num_read) / ((float)statbuf.st_size) * 100.0f;
        if (current_percent >= percentage + 10)
        {
            printf("%d%% complete\n", current_percent);
            percentage = current_percent;
        }
        // Read in another block
        uint8_t *offset = data + num_read;

        // Wait for available worker thread
        int worker_idx;
        int flag = 0;
        while ((worker_idx = worker_available(all_worker_args, NUM_THREADS)) == -1);

        debug_printf("Found available worker %d\n", worker_idx);

        // Allocate new job pointer if needed
        if (all_worker_args[worker_idx].curr_job == NULL)
        {
            debug_printf("Allocating job pointer\n");
            all_worker_args[worker_idx].curr_job = calloc(1, sizeof(job_ll*));
        }
        // Get a job for the worker
        debug_printf("Getting free job for worker %d\n", worker_idx);
        (*all_worker_args[worker_idx].curr_job) = get_free_job(&job_ll_head);
        start_flags[worker_idx] = 1;


        // Dispatch new job
        size_t input_size = fmin(BLOCK_SIZE, statbuf.st_size - num_read);
        num_read += input_size;
        (*(all_worker_args[worker_idx].curr_job))->block_num = curr_input_block++;
        (*(all_worker_args[worker_idx].curr_job))->in_data = offset;
        (*(all_worker_args[worker_idx].curr_job))->in_data_length = input_size;
        (*(all_worker_args[worker_idx].curr_job))->in_data_ready = 1;
    }

    // Tell all threads to exit and join them
    for (int i = 0; i < NUM_THREADS; i++)
    {
        exit_flags[i] = 1;
        printf("Ending worker %d\n", i);
        ret = pthread_join(threads[i], NULL);
        if (ret != 0)
        {
            fprintf(stderr, "pthread error\n");
            exit(1);
        }
    };

    // Join the output thread
    *output_exit = 1;
    pthread_join(output_thread_handle, NULL);

    handle_completion(job_ll_head, &curr_output_block, output_fd);
    printf("100%% complete\n");

    debug_printf("curr_output: %ld\n", curr_output_block);
    debug_printf("curr_input: %ld\n", curr_input_block);

    // Clean up all the memory
    close(fd);
    close(output_fd);


    // Clean up the linked list of jobs
    job_ll *curr = job_ll_head;
    job_ll *next;

    while (curr != NULL)
    {
        next = curr->next;
        if (curr->out_data != NULL)
        {
            free(curr->out_data);
        }
        free(curr);
        curr = next;
    }

    // Clean up all the pointers for the worker threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (all_worker_args[i].curr_job != NULL)
        {
            free(all_worker_args[i].curr_job);
        }
    }
    free(all_worker_args);
    free(threads);
    free(exit_flags);
    free(start_flags);

    free(output_args.exit);

    munmap(data, statbuf.st_size);
    
    printf("Finished writing output file to %s\n", OUTPUT_FILENAME);
    printf("%lu bytes written\n", total_bytes_written);

    free(OUTPUT_FILENAME);

    printf("Compression ratio %.2f%%\n", ((double)total_bytes_written) / ((double)statbuf.st_size) * 100);
}