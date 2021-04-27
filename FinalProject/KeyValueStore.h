#ifndef KEY_VALUE_STORE_H
#define KEY_VALUE_STORE_H

#include <functional>
#include <iostream>
#include <list>
#include <shared_mutex>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#define INITIAL_SIZE 100
// TODO: MUTEX

template <class K, class V, class H = std::hash<K>>
class KeyValueStore
{
    typedef typename std::list<std::pair<K, V>> ll;
    typedef typename std::list<std::pair<K, V>>::iterator ll_itr;
  public:
    KeyValueStore();
    KeyValueStore(size_t initial_size);

    bool hasKey(K key);
    std::optional<V> get(K key);
    bool put(K key, V value);
    bool replace(K key, V value); // Should be easy to add
    bool del(K key);

    void print();

    //void serialize(); // TODO

  private:
    std::vector<std::list<std::pair<K, V>>> hash_arr;
    size_t hash_arr_size;
    std::shared_timed_mutex access_mutex;

    bool check_resize();            // TODO
    void resize(size_t new_size);   // TODO
    int hash(K key);
};

template <class K, class V, class H>
KeyValueStore<K, V, H>::KeyValueStore() : KeyValueStore(INITIAL_SIZE)
{

}

template <class K, class V, class H>
KeyValueStore<K, V, H>::KeyValueStore(size_t initial_size)
{
    hash_arr.resize(initial_size);
    hash_arr_size = initial_size;
}

template <class K, class V, class H>
bool KeyValueStore<K, V, H>::hasKey(K key)
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

template <class K, class V, class H>
std::optional<V> KeyValueStore<K, V, H>::get(K key)
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

template <class K, class V, class H>
bool KeyValueStore<K, V, H>::put(K key, V value)
{
    std::unique_lock<std::shared_timed_mutex> lock(access_mutex);
    size_t h = hash(key);
    if (hasKey(key))
    {
        return false;
    }

    hash_arr[h].push_back(std::make_pair<K, V>(std::forward<K>(key), std::forward<V>(value)));

    // TODO: RESIZE

    return true;
}

template <class K, class V, class H>
bool KeyValueStore<K, V, H>::del(K key)
{
    std::unique_lock<std::shared_timed_mutex> lock(access_mutex);
    size_t h = hash(key);
    for (ll_itr itr = hash_arr[h].begin(); itr != hash_arr[h].end(); itr++)
    {
        if (key == itr->first)
        {
            hash_arr[h].erase(itr);
            return true;
        }
    }
    return false;
}

template <class K, class V, class H>
void KeyValueStore<K, V, H>::print()
{
    std::shared_lock<std::shared_timed_mutex> lock(access_mutex);
    size_t bin_num = 0;
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

template <class K, class V, class H>
int KeyValueStore<K, V, H>::hash(K key)
{
    return H()(key) % hash_arr_size;
}


#endif