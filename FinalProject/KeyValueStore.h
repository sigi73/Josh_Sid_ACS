#ifndef KEY_VALUE_STORE_H
#define KEY_VALUE_STORE_H

#include "DefaultSerializer.h"
#include <json/json.h>

#include <functional>
#include <fstream>
#include <iostream>
#include <list>
#include <shared_mutex>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define INITIAL_SIZE 100
#define SIZE_THRESHOLD 0.65


template <class K, class V, class H = std::hash<K>, class SK = DefaultSerializer<K>, class SV = DefaultSerializer<V>>
class KeyValueStore
{
    typedef typename std::list<std::pair<K, V>> ll;
    typedef typename std::list<std::pair<K, V>>::iterator ll_itr;
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

  private:
    std::vector<std::list<std::pair<K, V>>> hash_arr;
    size_t num_elements;
    size_t hash_arr_size;
    std::shared_timed_mutex access_mutex;

    bool should_resize();
    void resize(size_t new_size);
    int hash(K key);
};

template <class K, class V, class H, class SK, class SV>
KeyValueStore<K, V, H, SK, SV>::KeyValueStore() : KeyValueStore(INITIAL_SIZE) {}

template <class K, class V, class H, class SK, class SV>
KeyValueStore<K, V, H, SK, SV>::KeyValueStore(size_t initial_size)
{
    hash_arr.resize(initial_size);
    hash_arr_size = initial_size;
}

template <class K, class V, class H, class SK, class SV>
KeyValueStore<K, V, H, SK, SV>::KeyValueStore(std::string fn)
{
    std::cout << "here1" << std::endl;
    std::ifstream in_file;
    in_file.open(fn);
    Json::Value root;
    in_file >> root;

    std::cout << "here2" << std::endl;

    // Make sure all values are there
    if (!root.isMember("num_elements") ||
        !root.isMember("hash_arr_size") ||
        !root.isMember("arr"))
    {
        throw std::invalid_argument("Missing json members");
    }

    std::cout << "here3" << std::endl;

    hash_arr_size = root["hash_arr_size"].asInt();
    num_elements = root["num_elements"].asInt();


    Json::Value arr = root["arr"];
    std::cout << hash_arr_size << std::endl;
    std::cout << arr.size() << std::endl;
    if (arr.size() != hash_arr_size)
    {
        throw std::invalid_argument("Num elements disagrees with array size");
    }

    std::cout << "here4" << std::endl;

    hash_arr.resize(hash_arr_size);
    int num_loaded;
    for (int i = 0; i < hash_arr_size; i++)
    {
        Json::Value bin = arr[i];
        if (!bin.isNull());
        {
            for (int j = 0; j < bin.size(); j++)
            {
                K key = SK::deserialize(bin[j]["key"]);
                V value = SV::deserialize(bin[j]["value"]);
                // Verify Hash is correct
                if (i != hash(key))
                {
                    throw std::invalid_argument("Stored hash doesn't match actual hash");
                }
                hash_arr[i].push_back(std::make_pair<K, V>(std::forward<K>(key), std::forward<V>(value)));
                ++num_loaded;
            }
        }
    }
    std::cout << "here5" << std::endl;
    if (num_loaded != num_elements)
    {
        throw std::invalid_argument("Num of elements loaded does not equal number of saved elements");
    }
}

template <class K, class V, class H, class SK, class SV>
bool KeyValueStore<K, V, H, SK, SV>::hasKey(K key)
{
    std::shared_lock<std::shared_timed_mutex> lock(access_mutex);
    size_t h = hash(key);
    for (ll_itr itr = hash_arr[h].begin(); itr != hash_arr[h].end(); itr++)
    {
        if (key == itr->first)
        {
            return true; 
        }
    }

    return false;
}

template <class K, class V, class H, class SK, class SV>
std::optional<V> KeyValueStore<K, V, H, SK, SV>::get(K key)
{
    std::shared_lock<std::shared_timed_mutex> lock(access_mutex);
    size_t h = hash(key);
    for (ll_itr itr = hash_arr[h].begin(); itr != hash_arr[h].end(); itr++)
    {
        if (key == itr->first)
        {
            return itr->second;
        }
    }
    return {};
}

template <class K, class V, class H, class SK, class SV>
bool KeyValueStore<K, V, H, SK, SV>::put(K key, V value)
{
    std::unique_lock<std::shared_timed_mutex> lock(access_mutex);
    size_t h = hash(key);
    if (hasKey(key))
    {
        return false;
    }

    hash_arr[h].push_back(std::make_pair<K, V>(std::forward<K>(key), std::forward<V>(value)));
    ++num_elements;

    if (should_resize())
    {
        //std::cout << "Resizing" << std::endl;
        resize(2 * hash_arr_size);
    }

    return true;
}

template <class K, class V, class H, class SK, class SV>
bool KeyValueStore<K, V, H, SK, SV>::replace(K key, V value)
{
    std::unique_lock<std::shared_timed_mutex> lock(access_mutex);
    size_t h = hash(key);
    bool found = false;
    for (ll_itr itr = hash_arr[h].begin(); itr != hash_arr[h].end(); itr++)
    {
        if (key == itr->first)
        {
            found = true;
            itr->second = value;
            return true;
        }
    }
    // Key not found, just add it
    return put(key, value);
}

template <class K, class V, class H, class SK, class SV>
bool KeyValueStore<K, V, H, SK, SV>::del(K key)
{
    std::unique_lock<std::shared_timed_mutex> lock(access_mutex);
    size_t h = hash(key);
    for (ll_itr itr = hash_arr[h].begin(); itr != hash_arr[h].end(); itr++)
    {
        if (key == itr->first)
        {
            hash_arr[h].erase(itr);
            --num_elements;
            return true;
        }
    }
    return false;
}

template <class K, class V, class H, class SK, class SV>
void KeyValueStore<K, V, H, SK, SV>::print()
{
    std::shared_lock<std::shared_timed_mutex> lock(access_mutex);
    size_t bin_num = 0;
    std::cout << num_elements << " elements" << std::endl;
    for (auto const &bin : hash_arr)
    {
        std::cout << bin_num << ": ";
        for (auto const &kv : bin)
        {
            std::cout << "(K:" << kv.first << ",V:" << kv.second << "),";
        }
        std::cout << std::endl;
        bin_num++;
    }
}

template <class K, class V, class H, class SK, class SV>
void KeyValueStore<K, V, H, SK, SV>::serialize(std::string fn)
{
    std::unique_lock<std::shared_timed_mutex> lock(access_mutex);

    Json::Value root;
    root["num_elements"] = num_elements;
    root["hash_arr_size"] = hash_arr_size;

    Json::Value arr;
    int bin_num = 0;
    for (auto const &bin : hash_arr)
    {
        Json::Value json_bin;
        int bin_idx = 0;
        for (auto const &kv : bin)
        {
            json_bin[bin_idx]["key"] = SK::serialize(kv.first);
            json_bin[bin_idx]["value"] = SV::serialize(kv.second);
            bin_idx++;
        }
        arr[bin_num] = json_bin;
        bin_num++;
    }
    root["arr"] = arr;

    std::ofstream out_file;
    out_file.open(fn);
    out_file << root;
    out_file.close();
}

template <class K, class V, class H, class SK, class SV>
int KeyValueStore<K, V, H, SK, SV>::hash(K key)
{
    return H()(key) % hash_arr_size;
}

template <class K, class V, class H, class SK, class SV>
bool KeyValueStore<K, V, H, SK, SV>::should_resize()
{
    if ((float)num_elements / (float)hash_arr_size > SIZE_THRESHOLD)
        return true;
    else
        return false;
}

template <class K, class V, class H, class SK, class SV>
void KeyValueStore<K, V, H, SK, SV>::resize(size_t new_size)
{
    std::vector<std::list<std::pair<K, V>>> temp = hash_arr;
    hash_arr.clear();
    hash_arr.resize(new_size);
    hash_arr_size = new_size;
    num_elements = 0;

    for (auto const &bin : temp)
    {
        for (auto const &kv : bin)
        {
            put(kv.first, kv.second);
        }
    }
}



#endif