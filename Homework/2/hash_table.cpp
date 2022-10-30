#include "hash_table.h"

unsigned long hash(std::string str)
{
    unsigned long res = 5381;
    int i = 0, c;
    while (str[i] != '\0')
    {
        c = str[i++];
        res = ((res << 5) + res) + c; /* res * 33 + c */
    }
    return res;
}

template <typename T>
HashTable<T>::HashTable()
{
    max_hash = MAX_HASH;
    buckets = new std::list< std::pair<std::string, T> >[max_hash];
}

template <typename T>
HashTable<T>::HashTable(int max_hash)
{
    this->max_hash = max_hash;
    buckets = new std::list< std::pair<std::string, T> >[max_hash];
}

template <typename T>
HashTable<T>::~HashTable()
{
    delete[] buckets;
}

template <typename T>
bool HashTable<T>::hasKey(std::string key)
{
    int index = hash(key) % max_hash;
    for(auto it = buckets[index].begin(); it != buckets[index].end(); ++it)
        if(it->first == key) return true;
    return false;
}

template <typename T>
void HashTable<T>::insert(std::pair<std::string, T> p)
{
    int index = hash(p.first) % max_hash;
    auto it = buckets[index].begin();
    for(; it != buckets[index].end(); ++it)
        if(it->first == p.first)
        {
            it->second = p.second;
            break;
        }
    if(it == buckets[index].end())
    {
        buckets[index].push_back(p);
        cnt_elements++;
    }
}

template <typename T>
void HashTable<T>::remove(std::string key)
{
    int index = hash(key) % max_hash;
    for(auto it = buckets[index].begin(); it != buckets[index].end(); ++it)
        if(it->first == key)
        {
            buckets[index].remove(*it);
            cnt_elements--;
            break;
        }
}

template <typename T>
T HashTable<T>::get(std::string key)
{
    int index = hash(key) % max_hash;
    for(auto it = buckets[index].begin(); it != buckets[index].end(); ++it)
        if(it->first == key) return it->second;
    return T();
}

template <typename T>
int HashTable<T>::size()
{
    return cnt_elements;
}

template class HashTable<int>;