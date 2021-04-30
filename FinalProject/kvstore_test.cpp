#include "KeyValueStore.h"
#include "CustomKeyValues.h"

#include <fstream>
#include <iostream>
#include <string>


int main()
{
    KeyValueStore<int, std::string> store(10);

    std::cout << "Hello World" << std::endl;

    store.put(100, "Hello");
    store.put(8, "World");
    store.put(101, "World Fixed");
    store.put(1, "test");
    store.put(12, "test1");
    store.put(10, "test2");
    store.put(9, "test3");
    store.put(0, "test4");

    store.print();

    store.replace(0, "replaced");

    std::cout << "Get key 8: " << store.get(8).value_or("no_key") << std::endl;
    std::cout << "Delete key 8: " << (store.del(8) ? "delete successful" : "delete failed") << std::endl;
    std::cout << "Get key 8: " << store.get(8).value_or("no_key") << std::endl;

    store.put(118, "Goodbye");
    store.print();

    store.serialize("out.json");


    KeyValueStore<CustomKey, CustomValue, CustomKeyHash, CustomKeySerializer, CustomValueSerializer> customStore(10);

    customStore.put(CustomKey(1, 100), CustomValue(1, "name1", 0.2f));
    customStore.put(CustomKey(2, 100), CustomValue(9, "name2", 0.8f));
    customStore.put(CustomKey(3, 100), CustomValue(3, "name3", 1.2f));
    customStore.put(CustomKey(1, 200), CustomValue(2, "name4", 108.2f));

    std::cout << "Custom Store" << std::endl;
    customStore.print();
    customStore.serialize("out_custom.json");
}