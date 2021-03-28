#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//std::unordered_set<std::string> process_file(std::string fn, size_t byte_offset,
void process_file(std::string fn, size_t byte_offset,
    size_t length, int index, std::vector<std::unordered_set<std::string>> &sets) {
    std::ifstream input_file;
    input_file.open(fn);
    input_file.seekg(byte_offset);

    auto start_pos = input_file.tellg();

    //std::cout << byte_offset << " " << length << " " << std::endl;
    

    // Seek to next newline
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

    // Read newlines into set until end of segment
    while (input_file.tellg() - start_pos < length) {
        std::string line;
        std::getline(input_file, line); 
        sets[index].insert(line);
    }

    input_file.close();

    std::cout << sets[index].size() << std::endl;

}

void usage(const char *executable_name) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t$ " << executable_name << " input_file num_threads" << std::endl;
}

void output_file(std::string input_fn, std::string out_fn,
    std::unordered_map<std::string, int> &map, std::vector<std::string> &words)
{
    std::ofstream output_file;
    output_file.open(out_fn);

    output_file << map.size() << std::endl;
    for (auto word : words)
    {
        output_file << word << std::endl;
    }

    std::ifstream input_file;
    input_file.open(input_fn);

    std::string line;
    while (std::getline(input_file, line)) {
        output_file << map[line] << std::endl;
    }
    input_file.close();
    output_file.close();

}

int main(int argc, const char **argv) {
    if (argc != 3) {
        usage(argv[0]);
        return 1;
    }

    int num_threads;
    try {
        num_threads = std::stoi(argv[2]);
    } catch(std::exception exc) {
        std::cerr << "Error reading num threads" << std::endl;
        usage(argv[0]);
        return 1;
    }

    // https://www.bfilipek.com/2019/01/filesize.html
    std::error_code ec{};
    auto file_size = std::filesystem::file_size(argv[1], ec);
    //std::cout << "Size: " << file_size << std::endl;
    if (ec != std::error_code{}) {
        std::cerr << "Error getting size of file" << std::endl;
        return 1;
    }

    std::vector<std::thread> threads;
    std::vector<std::unordered_set<std::string>> sets;
    for (int i = 0; i < num_threads; i++)
    {
        int length = file_size / num_threads;
        int offset = i * length;
        if (i == num_threads - 1)
        {
            length = file_size - offset;
        }
        std::unordered_set<std::string> set;
        sets.push_back(std::move(set));

        std::thread thread(process_file, argv[1], offset, length, i, std::ref(sets));
        threads.push_back(std::move(thread));
    }

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
    output_file(argv[1], "test_out.txt", word_to_num, words);

    return 0;
}