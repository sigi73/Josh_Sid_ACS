#include "KeyValueStore.h"

#include <iostream>
#include <string>

int main()
{
    KeyValueStore<int, std::string> store(18);

    std::cout << "Hello World" << std::endl;

    store.put(100, "Hello");
    store.put(8, "World");
    store.put(101, "World Fixed");

    store.print();

    std::cout << "Get key 8: " << store.get(8).value_or("no_key") << std::endl;
    std::cout << "Delete key 8: " << (store.del(8) ? "delete successful" : "delete failed") << std::endl;
    std::cout << "Get key 8: " << store.get(8).value_or("no_key") << std::endl;

    store.put(118, "Goodbye");
    store.print();

}