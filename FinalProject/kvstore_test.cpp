#include "KeyValueStore.h"
#include "CustomKeyValues.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <algorithm>
#include <unistd.h>
#include <chrono>



// random generator function:
int myrandom(int i) { return std::rand()% i;}

std::vector<int> generateRandomIDs(int size){
    //Generating vector of ID's for test
        //need to keep list so can grab id's for testing
    std::vector<int> idContainer;
    for (size_t i = 0; i < size; i++)
    {
        idContainer.push_back(i);
    }
    //making vector random
    std::random_shuffle ( idContainer.begin(), idContainer.end(), myrandom);
    return idContainer;
}

std::string generateRandomString(){
    static const char alphanum[] =
    "0123456789"
    "!@#$%^&*"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

    std::string temp_str;
    for(unsigned int i = 0; i < 20; ++i){
        temp_str += alphanum[1 + rand() % sizeof(alphanum) -  1 ];
    }
    return temp_str;
}

//Test 1 tests basic put, get, replace, and delete resize for functionality
void singleThreadTest1(){
    KeyValueStore<int, std::string> test1(10);
    test1.put(100, "Hello");
    test1.put(8, "World");
    test1.put(101, "World Fixed");
    test1.put(1, "test");
    test1.put(12, "test1");
    test1.put(10, "test2");
    test1.put(9, "test3");
    test1.put(0, "test4");

    test1.print();
    test1.replace(0, "replaced");

    std::cout << "Get key 8: " << test1.get(8).value_or("no_key") << std::endl;
    std::cout << "Delete key 8: " << (test1.del(8) ? "delete successful" : "delete failed") << std::endl;
    std::cout << "Get key 8: " << test1.get(8).value_or("no_key") << std::endl;

    test1.put(118, "Goodbye");
    test1.print();

    test1.serialize("out.json");
}

//tests the functionality of the custom version of key value store
void singleThreadTest2(){
    KeyValueStore<CustomKey, CustomValue, CustomKeyHash, CustomKeySerializer, CustomValueSerializer> customStore(10);

    customStore.put(CustomKey(1, 100), CustomValue(1, "name1", 0.2f));
    customStore.put(CustomKey(2, 100), CustomValue(9, "name2", 0.8f));
    customStore.put(CustomKey(3, 100), CustomValue(3, "name3", 1.2f));
    customStore.put(CustomKey(1, 200), CustomValue(2, "name4", 108.2f));

    std::cout << "Custom Store" << std::endl;
    customStore.print();
    customStore.serialize("out_custom.json");
}


void singleThreadTest3(int insert_number){
    std::vector<int> _idContainer = generateRandomIDs(insert_number);

    // for(unsigned i = 0; i< _idContainer.size(); ++i) {
    //     std::cout << _idContainer[i] << std::endl;
    // }

    //timing to insert insert_number values from a table of size 10
    KeyValueStore<int, std::string> test3(10);
    std::string temp_string;
    //generating words to associate with id's
    auto start =  std::chrono::high_resolution_clock::now();  // start timer for the beginning of testing placeing items in table and resizing time effect
    auto time_difference = std::chrono::duration_cast<std::chrono::microseconds>(start - start); // accounting for time to generate string
    for(unsigned i = 0; i< _idContainer.size(); ++i) {
        auto word_start =  std::chrono::high_resolution_clock::now();  //start of generating string time
        temp_string = generateRandomString();
        //std::cout << temp_string<< std::endl;
        auto word_stop =  std::chrono::high_resolution_clock::now();//end of generating string time
        //placing value in table with the key that is in the index of the iterator of the loop
        test3.put(_idContainer[i],temp_string);
        time_difference +=  std::chrono::duration_cast<std::chrono::microseconds>(word_stop - word_start); //keeping track of difference
    }
    auto stop =  std::chrono::high_resolution_clock::now(); // stop of testing time
    auto duration =  std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 
    auto total_time = duration - time_difference;// is actual time taken
  
    std::cout << "Time taken to place "<< insert_number << " items in table: "
         << total_time.count() << " microseconds" << std::endl;

         
    //start test of replacing items in table
    //going through insert_number/4 number of replacements
    start =  std::chrono::high_resolution_clock::now();  // start timer for the beginning of testing placeing items in table
    for (size_t i = 0; i < insert_number/4; i++)
    {
            test3.replace(0, "replaced");
    }
    stop =  std::chrono::high_resolution_clock::now(); // stop of testing time
    duration =  std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 
    std::cout << "Time taken to replace "<< insert_number / 4 << " items in table: "
         << duration.count() << " microseconds" << std::endl;

    //start test of getting items in table
    start =  std::chrono::high_resolution_clock::now();  // start timer for the beginning of testing getting items in table 
    std::string test_string;
    for (size_t i = insert_number/4; i < insert_number/2; i++)
    {
      //  std::cout << "Get key "<< i <<" : " << test3.get(i).value_or("no_key") << std::endl;
      test_string = test3.get(i).value_or("no_key"); 
      if(test_string.compare("no_key") == 0){
        std::cout << "failed to retrieve item with key "<< i << " in single threaded test 3" << std::endl;
      }

    }
    stop =  std::chrono::high_resolution_clock::now(); // stop of testing time
    duration =  std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 

    std::cout << "Time taken to find "<< insert_number / 4 << " items in table: "
         << duration.count() << " microseconds" << std::endl;

    //start test of deleting

    start =  std::chrono::high_resolution_clock::now();  // start timer for the beginning of testing deleting items in table 
    for (size_t i = 0; i < insert_number - 1; i += 2)
    {
        test3.del(i);
    }
    stop =  std::chrono::high_resolution_clock::now(); // stop of testing time
    duration =  std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 

    std::cout << "Time taken to delete "<< insert_number / 2 << " items in table: "
         << duration.count() << " microseconds" << std::endl;


   // test3.serialize("out.json");
}

void thread_func(KeyValueStore<int, float> &store, int starting_idx, int ending_idx, int replace_idx, int tid)
{
    float num;
    float LO = -100.f;
    float HI = 100.f;
    for (int i = starting_idx; i < ending_idx; i++)
    {
        // https://stackoverflow.com/questions/686353/random-float-number-generation
        num = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
        store.put(i, num);
    }
    store.replace(replace_idx, (float)tid);
}


void multiThreadTest()
{
    KeyValueStore<int, float> store(10);
    std::thread t1(thread_func, std::ref(store), 0, 10, 15, 1);
    std::thread t2(thread_func, std::ref(store), 10, 20, 25, 2);
    std::thread t3(thread_func, std::ref(store), 20, 30, 35, 3);
    std::thread t4(thread_func, std::ref(store), 30, 40, 5, 4);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    store.print();
}

void usage(char *argv[])
{
    std::cout << "Usage:" << std::endl;
    std::cout << "$ " << argv[0] << " command" << std::endl;
    std::cout << "\tcommand can be one of the following:" << std::endl;
    std::cout << "\t\tbasic: run a basic test with standard datatypes" << std::endl;
    std::cout << "\t\tcustom: run a test with custom datatypes" << std::endl;
    std::cout << "\t\tperformance: run a performance test" << std::endl;
    std::cout << "\t\tmultithreaded: run a multithreaded test" << std::endl;
}

int main(int argc, char *argv[])
{
    /*
    if (argc == 1)
    {
        usage(argv);
    }
    else
    {
        if (strcmp(argv[1], "basic") == 0)
        {
            singleThreadTest1();
        }
        else if (strcmp(argv[1], "custom") == 0)
        {
            singleThreadTest2();
        }
        else if (strcmp(argv[1], "performance") == 0)
        {
            singleThreadTest3(1000000);
        }
        else if (strcmp(argv[1], "multithreaded") == 0)
        {
            multiThreadTest();
        }
        else
        {
            std::cerr << "Command not recognized" << std::endl;
            usage(argv);
        }
    }
    */
   multiThreadTest();
    // singleThreadTest2();
    //big boy test for timing of inserting, replacing, getting, and deleting
        //tests are done with string but may be done with any type contained 
    //singleThreadTest3(1000000);
    //multithreaded_test();
}
