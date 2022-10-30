#ifndef __HASH_TABLE_H__
#define __HASH_TABLE_H__

#include <string>
#include <list>
#define MAX_HASH 10000000

template <typename T>
class HashTable
{
private:
    int cnt_elements = 0;
    std::list< std::pair<std::string, T> > *buckets;
    int max_hash;

public:
    HashTable();
    HashTable(int max_hash);
    ~HashTable();

    bool hasKey(std::string key);
    void insert(std::pair<std::string, T>);
    void remove(std::string key);
    T get(std::string key);
    int size();
};

#endif // __HASH_TABLE_H__