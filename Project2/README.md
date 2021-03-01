# File Compression with Multiple Threads

## Implementation

### Overview
This program makes use of the linux pthread library for multithreaded programming. Per the assignment instructions, the input file is divided into 4096 byte blocks for seperate threads to compress using the zlib library. The compressed results are then combined into an output file using an output thread.

The input file is read using mmap as a read only file to load the file into memory. Since mmap is used as `O_RDONLY`, `PROT_READ`, and `MAP_PRIVATE`, sections of the file are only loaded into memory as needed, not all at once. This means that files larger than the available memory can be compressed without issue.

The main thread dispatches compression tasks to the worker threads using the `job_ll` data structure. This structure is an element of a linked list, and the resoning for this is described below. An output thread takes completed jobs, and outputs them in the correct order to the output file.

### Job Data Structure
Worker threads are assigned jobs using the `job_ll` data structure.

```
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
```
The flags `job_available`, `in_data_ready`, and `out_data_ready`, are using for signaling to show if the job is available to be used, if the input data is ready, and if the output data is ready. `in_data` is the pointer to the input data block, with a length of `in_data_length`. `out_data` is the pointer to the output data block, with an allocated size of `out_data_allocated`. However, the actual length of the data is in `out_data_length`. `block_num` indicates the block index of the input data, and is used for ordering the output. `next` is a pointer to another job in the linked list.

Jobs are ordered in a linked list. This allows us to create jobs and pass them to worker threads. When the worker thread finishes compression, it may not be the next block to output. As a result, we store the job in a linked list and give the worker thread a new job. The output thread will scan through the linked list and output blocks in order as they are completed. Once a block is outputted, the job can be re-used. This helps minimize extra allocations.

### Verifying correctness
To verify our code is working as expected, we first developed the threading without compression. We instead copy the input to the output buffer, and ensure that the output file is identical to the input file. This makes sure that blocks are being output in the correct order. In addition, `valgrind` was used on a small input file to validate that there are no memory leaks. 

### Worker Threads
The main worker thread is in the `void *worker_thread(void *args)` function. The structure `worker_args` is used to pass arguments. `curr_job` is the current job. `exit` is an flag to allow the worker thread to exit. `start` indicates when to start the next job. `worker_num` is used for printing the worker number.

The worker threads wait for new jobs from the main thread, then perform the compression. If this job already has an output buffer allocated, then it is re-used unless it needs to be made bigger, in which case `realloc` is used to increase the size. If this is the first use of the job, then `malloc` is used to generate the buffer.

```
typedef struct worker_args_t {
    job_ll **curr_job; // Pointer to pointer of current job. Pointer of a pointer so that we can update the job
    char *exit; // Indicator when work is done to exit
    char *start; // Indicator when to start the next job
    int worker_num; // Save worker number for debugging/printing
} worker_args
```

### Ouput thread
The output thread reads repeatedley through the list of jobs, and outputs the blocks in order. As blocks have been outputted, their corresponding jobs can then be re-used by the main thread for new jobs for worker threads. This system minimizes the downtime for worker threads, by giving them new jobs immediately while making sure that the blocks are still output in the correct order. This comes at the cost of higher memory use.

## Compiling and Running the Code

### gcc on Linux
1. Change to the `Project2` directory
2. Test short matrix multiplication: `gcc -lm -lz -lpthread compression.c -o project2`

### Generating Test Files
Test files of random text data can be generated using the following unix command:

`base64 /dev/urandom | head -c 1G > 1G_base64.txt`

This command generates 1 gigabyte of random text data. The parameter passed to `head` can be changed for different sizes.

### Running the code
The first argument is the path to the input file. The output file will be created in the same directory. The second argument is the number of worker threads. For example, to compress the file `1G_base64.txt` in the same directory with 4 worker threads, run

```
$ ./project2 1G_base64.txt 4
```

## Test Results
The code was run using a 10GB input file. The file was placed on three different storage mediums, a Hard Disk Drive (HDD), Solid State Drive (SSD), and in a RAMdisk. The results for each are shown below

### HDD
TODO: Insert tables and graphs

### SSD
TODO: Insert tables and graphs

### Ramdisk
TODO: Insert tables and graphs

### Analysis and Conclusion
TODO: Best number of threads for each case.

We observe that the performance scales much better with number of threads when the input file is on a faster storage medium. This suggests that, unsurprisingly, compression is not compute but IO bound. Most of the time is spent getting the data to and from the storage medium, and therefore not as much performance improvement is seen from increasing the level of parallelism.