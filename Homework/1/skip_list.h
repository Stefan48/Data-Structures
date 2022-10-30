#ifndef __SKIP_LIST__H__
#define __SKIP_LIST__H__

template <typename T>
struct Node
{
    Node();
    Node(T info);

    Node *prev, *next;
    Node *below, *above;
    T info;
    int level;
};

template <typename T>
class Skiplist
{
public:
    Skiplist(T lower_bound, T upper_bound);
    ~Skiplist();
    Skiplist(const Skiplist& other);
    Skiplist& operator=(const Skiplist& other);
	// getters
    int GetSize();
    int GetHeight();
    Node<T>* GetHead();
    Node<T>* GetTail();
	// operations
    Node<T>* Search(T key);
    void Insert(T key);
    void Delete(T key);
    void PrintList();

private:
    T lower_bound, upper_bound;
    Node<T> *head, *tail;
    int size;
    int height;
};
#endif // __SKIPLIST__H__