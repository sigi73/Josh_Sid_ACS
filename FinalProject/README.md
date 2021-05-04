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
    bool replace(K key, V value);
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
`g++ kvstore_test.cpp jsoncpp.cpp -std=c++11 -lpthread`.
`g++ deserialize_test.cpp jsoncpp.cpp -std=c++11 -lpthread`

## Performance Testing

All tables for testing were generated with key values within a specific range randomly assigned given to randomly generated alphanumeric strings. 

System specs:
CPU: i7-8700k at 4.5GHz
RAM: 32GB (4x8GB) 3200MHz Corsair Vengeance LPX

### Insert Performance
Inserting into the table was done in sigle threaded mode as there is no benefit to multithreading due to only one object being able to be inserted into the table at a time. Therefore there should be no performace gain expected. 

| Number of Inserts  | Time to Complete (in Micro Seconds)	| 
| :----: |  :----:   | 
|1000    | 2031      |
|2500    | 3570	     | 
|5000	   | 8995	     |
|10000   | 17015     | 
|20000   | 29475     | 
|30000   | 54256     | 
|40000   | 61004     |
|50000   | 65548     |
|75000   | 116779    |
|100000  | 131784    |
|250000  | 472532    |
|500000  | 998278    |
|750000  | 1142650   |
|1000000 | 2001398   |

<img src="https://cdn.discordapp.com/attachments/366025595257225229/838840642721415179/Insert_Graph.JPG" width="700">

### Replace and  Single-threaded Look-up Performance
The number of operations performed for both replace and look-up were both 25% of the size of the table. The range for which the replace was done was the first 25% of the range of key values and the look-up was done on the subsequent 25%.

| Size of Table  | Number of Operations | Replacement Time (in Micro Seconds)	| Look-up Time (in Micro Seconds)	|
| :----: |  :----:   |  :----:   | :----:   | 
|1000    | 250       |34         |40        |
|2500    | 625	     |78         |78        |
|5000	   | 1250	     |161        |181       |
|10000   | 2500      |273        |325       |
|20000   | 5000      |523        |708       |
|30000   | 7500      |788        |982       |
|40000   | 10000     |1044       |1473      |
|50000   | 12500     |1308       |1969      |
|75000   | 18750     |1948       |2952      |
|100000  | 25000     |2683       |4101      |
|250000  | 62500     |6790       |8710      |
|500000  | 125000    |13178      |17626     |
|750000  | 187500    |19884      |32314     |
|1000000 | 250000    |26471      |35379     |

<img src="https://cdn.discordapp.com/attachments/366025595257225229/838847532603408414/ReplaceLookup_Graph.JPG" width="700">

### Effect of Multithreading on Performance
Because multithreading cannot be utilized on operations that change the database (i.e. insert, replace, delete), the operation chosen to test multithreading was the Look-up. The two and four threaded tests were performed with the same size tables and same number of key values to look up.

| Size of Table  | Number of Operations | Single-threaded Look-up Time (in Micro Seconds)	|Two threaded Look-up Time (in Micro Seconds)	| Four threaded Look-up Time (in Micro Seconds)	|
| :----: |  :----:   | :----:   | :----:   | :----:   | 
|1000    | 250       |40        |105       |296       |
|2500    | 625	     |78        |139       |322       |
|5000	   | 1250	     |181       |225       |292       |
|10000   | 2500      |325       |365       |411       |
|20000   | 5000      |708       |674       |589       |
|30000   | 7500      |982       |993       |946       |
|40000   | 10000     |1473      |1200      |1148      |
|50000   | 12500     |1969      |1527      |1385      |
|75000   | 18750     |2952      |2141      |1883      |
|100000  | 25000     |4101      |2963      |2488      |
|250000  | 62500     |8710      |10421     |7285      |
|500000  | 125000    |17626     |16060     |10892     |
|750000  | 187500    |32314     |24704     |17705     |
|1000000 | 250000    |35379     |28705     |22854     |
<img src="https://cdn.discordapp.com/attachments/366025595257225229/838860117348450374/unknown.png" width="700">

### Delete Performance
The number of operations performed for delete was 50% of the size of the table and every other key was chosen to be deleted.

| Size of Table  | Number of Operations | Delete Time (in Micro Seconds)	| 
| :----: |  :----:   |  :----:   |
|1000    | 500       |72         |
|2500    | 1250	     |124        |
|5000	   | 2500	     |306        |
|10000   | 5000      |543        |
|20000   | 10000     |1172       |
|30000   | 15000     |1627        |
|40000   | 20000     |2308       |
|50000   | 25000     |3241       |
|75000   | 37500     |4489       |
|100000  | 50000     |6780       |
|250000  | 125000     |14391       |
|500000  | 250000    |54854      |
|750000  | 375000    |19884      |
|1000000 | 500000    |57889      |
<img src="https://cdn.discordapp.com/attachments/366025595257225229/838859970823847987/Delete_Graph.JPG" width="700">

## Resultts and Conclusions

From testing, the inputing values into the hashtable has a complexity of O(n) which is ideal as that is what is expected inserting every value into the table. From this it can be determined that the impact of the resize does not effect the performace of inserting values. If it had, the trend of the curve would be more like O(nlog(n)) which would be unfavorable for very large inserts. For the replace vs singlethreaded lookup, the performance of the lookup is worse than that of the replace. This may be due to the implementation or more likely the check to ensure that the value retrieved is not null. While both of these trends are linear which is not the ideal performance of a hashtable replace and get operation which is constant. However due to the time scale being so small, in real world application, the get and replace operations would effectively be constant. This holds true for the delete operation as well which is also linear but also operates so fast it would effectively be constant. For the effect of multithreading, the benefit of using multiple threads is very apparent when the number of inserts is 40000. After this point it is also apparent that using 4 threads is faster than 2 threads which is still faster than a single threaded execution. It is expected however that if more threads were used there would be deminishing returns.

In conclustion, the implementation of this hashtable is acceptable and can be used effectively if one desires. However if one was to use this in a much higher traffic setting with multiple users athe the same time there may be issue encountered. The biggest issue is that there can be no operation done during a resize. This is mitigated in this implementation as resizing does not occur as often as in other implentations but it may still occur. In the future we hope to mitigate this by investigating other meathods of locking hashtables used in the field or investing in a way to make changes that then get brought into the resized table when it is finished resizing.

