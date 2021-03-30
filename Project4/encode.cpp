#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/*
 * Finds the unique words in a chunk of a file.
 * Reads a chunk of 'length' bytes, starting at 'byte_offset', from file with path 'fn'
 * Writes the unqiue words to the 'index' set in 'sets'
 */
void process_file(std::string fn, size_t byte_offset,
    size_t length, int index, std::vector<std::unordered_set<std::string>> &sets) {
    std::ifstream input_file;
    // Seek to the start of the chunk
    input_file.open(fn);
    input_file.seekg(byte_offset);

    // Record where we start, read to start_pos + length
    auto start_pos = input_file.tellg();

    // Seek to next newline. We don't want to start in the midle of a word
    if (byte_offset != 0) {
        // If the previous character was a newline, dont jump forward
        input_file.seekg(byte_offset - 1);
        char c;
        input_file.get(c);
        if (c != '\n') {
            std::cout << "Index " << index << " started in middle of line, seeking to next line" << std::endl;
            while (c != '\n') {
                input_file.get(c);
            }
        }
    }

    // Read newlines into set until end of segment. It's okay if the last line goes over, the next chunk will skip it
    while (input_file.tellg() - start_pos < length) {
        std::string line;
        // Get the current line and add it to the set
        std::getline(input_file, line); 
        sets[index].insert(line);
    }

    // Cleanup
    input_file.close();
}

/*
 * Prints helper usage
 */
void usage(const char *executable_name) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t$ " << executable_name << " input_file output_file num_threads" << std::endl;
}

/*
 * Ouptuts the encoded file to `out_fn`. Reads from `input_fn`, and uses
 * the mappings in `map` and `words`. `map` allows for reading from the input
 * file and mapping from string->integer. `words` is used to output the encoded
 * table and maps int->string
 */
void output_file(std::string input_fn, std::string out_fn,
    std::unordered_map<std::string, int> &map, std::vector<std::string> &words)
{
    // Open the output file
    std::ofstream output_file;
    output_file.open(out_fn);

    // First write the number of encoded words
    output_file << map.size() << std::endl;
    // Write each encoded word in order
    for (auto word : words)
    {
        output_file << word << std::endl;
    }

    // Open the input file
    std::ifstream input_file;
    input_file.open(input_fn);

    // Output the encoded version of each line to the output file
    std::string line;
    while (std::getline(input_file, line)) {
        output_file << map[line] << std::endl;
    }

    // Cleanup
    input_file.close();
    output_file.close();
}

int main(int argc, const char **argv) {
    // Check command line arguments
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }

    // Get the requested number of threads
    int num_threads;
    try {
        num_threads = std::stoi(argv[3]);
    } catch(std::exception exc) {
        std::cerr << "Error reading num threads" << std::endl;
        usage(argv[0]);
        return 1;
    }

    // https://www.bfilipek.com/2019/01/filesize.html
    // Get the size of the file to divide into chunks
    std::error_code ec{};
    auto file_size = std::filesystem::file_size(argv[1], ec);
    if (ec != std::error_code{}) {
        std::cerr << "Error getting size of file" << std::endl;
        return 1;
    }

    // Create a thread for each chunk
    std::vector<std::thread> threads;
    std::vector<std::unordered_set<std::string>> sets;
    for (int i = 0; i < num_threads; i++)
    {
        int length = file_size / num_threads;
        int offset = i * length;
        if (i == num_threads - 1)
        {
            // For the last thread, give it the remainder of the file
            length = file_size - offset;
        }
        // Create a new set for this worker thread
        std::unordered_set<std::string> set;
        sets.push_back(std::move(set));

        // Start the worker thread
        std::thread thread(process_file, argv[1], offset, length, i, std::ref(sets));
        threads.push_back(std::move(thread));
    }

    // Wait for the worker threads to finish
    for (std::thread &thread : threads)
    {
        thread.join();
    }

    // Combine all the sets
    std::unordered_map<std::string, int> word_to_num;
    std::vector<std::string> words;
    int word_index = 0;
    for (int i = 0; i < num_threads; i++)
    {
        for (auto &elem : sets[i])
        {
            if (word_to_num.find(elem) == word_to_num.end())
            {
                word_to_num[elem] = word_index++;
                words.push_back(elem);
            }
        }
    }

    // Output file
    output_file(argv[1], argv[2], word_to_num, words);

    return 0;
}