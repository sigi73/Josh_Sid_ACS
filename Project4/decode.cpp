#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>


void usage(const char *executable_name) {
    std::cout << "Usage:" << std::endl;
    std::cout << "\t$ " << executable_name << " decoded_file" << std::endl;
}

//gets how many times word is in file
void getwords(std::string input_fn, std::vector<std::string> &words, std::vector<int> &repetitions){

    std::ifstream input_file;
    input_file.open(input_fn);

    //getting number of words
    std::string number_of_words;
    std::getline(input_file, number_of_words);

    int word_count = stoi(number_of_words);

    std::string temp_word;
    //adding words to vector (order matters!)
    for (int i = 0; i < word_count; i = i + 1) {
        //std::cout << i << "\n";
        std::getline(input_file, temp_word);
        words.push_back(temp_word);

        //filling vector with empty values with same number of indexes as words
        repetitions.push_back(0);
    }   

    //test to make sure all words are there
    // for (size_t i = 0; i < word_count; i++)
    // {
    //     std::cout << words[i] << "\n";
    // }
    ///////   all words should be found now
        //    repetitions should be filled with same number of indexes as words
    
    //filling repetitions with number of repetitions per word at same index as word
    while (std::getline(input_file, temp_word)) {
        repetitions[stoi(temp_word)] += 1;
    }
    //test to make sure counts are correct
    // for (size_t i = 0; i < word_count; i++)
    // {
    //     std::cout << repetitions[i] << "\n";
    // }
    input_file.close();
 }

//printing everything that you pass it
 void print_mul_col(std::vector<std::string> &words, std::vector<int> &repetitions){
     //double check that they are the same
     if(words.size() != repetitions.size()){
        std::cout << "there was an error generating the results" << "\n";
        return;
     }

    for (size_t i = 0; i < words.size(); i++)
    {
        std::cout << words[i] << " - " << repetitions[i] << "\n";
    }

 }
//checks to see if word is in list, if so return index, otherwise return negative number
int is_word(std::vector<std::string> &words, std::string input_word){
    std::vector<std::string>::iterator it = std::find(words.begin(), words.end(), input_word);
    if (it != words.end()){
        int index = std::distance( words.begin(), it );
        return index;
    }
    return -1;
}
//printing one word
void print_repetition(const std::vector<std::string> &words, const std::vector<int> &repetitions, const int word_index){
    std::cout << words[word_index] << " appeared " << repetitions[word_index] << " times" << std::endl;
}

int main(int argc, const char **argv) {
    //if no arg then make sure that dummy knows what to input to this program
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }
    //reading encoded file
    //
    //2 vectors, one for words, other for count of words in same index as word in first vector
    //
    //passing to function to do all of the fun stuff
    std::vector<std::string> words;
    std::vector<int> repetitions;
    getwords(argv[1],words, repetitions);

    //printing output
    //print_mul_col(words,repetitions);

    std::cout << "To Query, please enter word you wish to see how many repititions for."<< "\n";
    std::cout << "To Query by the number associated with the word when encoded, enter number!" << "\n";
    std::cout << "To exit the program, enter exit!" << "\n";


    int input_index = 0;
    std::string input_word;
    while(1) 
    {   
        //get entry
        std::cout << "Please enter word:" << std::endl;
        std::cin >> input_word;
        //exit
        if (input_word == "exit!")
            break;
        //look to see if index is real, if so show word and repeats
        else if(input_word == "number!"){
              std::cout << "Please enter number:" <<  std::endl;
              std::cin >> input_index;
              if(input_index >= 0 || input_index < words.size()){
                  print_repetition(words,repetitions,input_index);
              }
              else{
                 std::cout << "Number entered was out of range of words" <<  std::endl;
              }
        }
        //else see if word exists, if it does then print it with repetitions
        else{
            input_index = is_word(words,input_word);
            if(input_index < 0){
                std::cout << "Word was not found" <<  std::endl;
            }
            else{
                print_repetition(words, repetitions, input_index);
            }
        }
    }
    
    return 0;
}
