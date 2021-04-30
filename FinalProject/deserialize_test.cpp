#include "KeyValueStore.h"
#include "CustomKeyValues.h"

#include <fstream>
#include <iostream>
#include <string>

int main()
{
    KeyValueStore<int, std::string> store("out.json");
    store.print();


    KeyValueStore<CustomKey, CustomValue, CustomKeyHash, CustomKeySerializer, CustomValueSerializer> custom_store("out_custom.json");
    custom_store.print();
}