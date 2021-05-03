# In Memory Key Value Store

## Project Overview
In this project we implement an in-memory key-value store using a hash-table indexing data structure. 

The store supports the GET, PUT, REPLACE, and DELETE operations. 
The store also supports concurrency control through mutual exclusion with mutexes. 
The hashtable implemention implements re-hashing to adjust the hash-table size as the data size changes.
The store is serializeable to a json data-structure.

The in-memory key-value store is a very commonly used indexing data structure for it's easy use to quickly access data from keys. 


## Implementation Details
### Public Interface
The code snippet below shows the public interface of the key value store. This class is templated, and therefore any type can be used for the key (K) and value (V). The standard std::hash is used for the default hash function on K, but can be replaced with a custom hash function. SK and SV are classes that define `static Json::Value serialize(T val);` and `static T deserialize(Json::Value json_val);` to serialize and deserialize the Key and Value classes to and from json.
```
template <class K, class V, class H = std::hash<K>, class SK = DefaultSerializer<K>, class SV = DefaultSerializer<V>>
class KeyValueStore
{
  public:
    KeyValueStore();
    KeyValueStore(size_t initial_size);
    KeyValueStore(std::string fn); // Deserialize from JSON

    bool hasKey(K key);
    std::optional<V> get(K key);
    bool put(K key, V value);
    bool replace(K key, V value); // Should be easy to add
    bool del(K key);

    void print();

    void serialize(std::string fn);
};
```

### Underlying implementation
#### Hash Table
The hash table is implemented as a `std::vector<std::list<std::pair<K, V>>>`. The vector is indexed by the hash of the key. The list is used as a linked list to handle key collision. The key is saved alongside the value to ensure that no duplicate keys are added. The hash table size can be increased dynamically. Whenever a PUT command is used, if the hash table is over 65% filled, the table size is doubled. The current table is copied, then resized, and each element in the old table is added to the new table. 

`std::hash` is used as the default hash function. If a custom type is used, the specialization of the hash function can be added to the std namespace to keep using `std::hash`. As an alternative, a custom class/struct can be created that defines `std::size_t operator(const K &k)` which returns the hash of the passed key. This can be passed as the `H` template as a new hash function. An example is shown in `struct CustomKeyHash` in *CustomKeyValues.h* for the custom key class `class CustomKey`. 

#### Concurrency Support
In order to support concurrent commands through `std::thread`, we make use of `std::shared_mutex`. This mutex has two modes, exclusive and shared. Functions that modify the table, such as put, delete, replace, resize, aquire the exclusive mutex. Functions that read the table, such as get or print acquire the shared mutex. That allows multiple functions to read the store at once, while ensuring that only one at a time can modify the table. This also ensures that modifications cannot happen while reading.

#### Serialization
Serialization is performed to JSON. JSON as a text format is used as it avoids issues with binary serialization such as endianess, and possible issues with bit alignment. The library [jsoncpp](https://github.com/open-source-parsers/jsoncpp) is used to handle parsing JSON. The default serializer supports integers, floats, and strings. For other types to be serialized, a cusom class should be passed to SK and SV for serializing the key and value respectively. 

## Compiling
### CMake
```
$ mkdir build
$ cd build
$ cmake ..
$ make final_test
$ make deserialize_test
```
### G++:
`g++ kvstore_test.cpp jsoncpp.cpp -std=c++11`

`g++ deserialize_test.cpp jsoncpp.cpp -std=c++11`
