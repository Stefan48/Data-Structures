#include <iostream>
#include "functions.h"
#include "driver.h"
#include "skip_list.h"

// default node constructor
template <typename T>
Node<T>::Node()
{
    prev = next = below = above = NULL;
    level = 0;
}
// other node constructor
template <typename T>
Node<T>::Node(T info)
{
    prev = next = below = above = NULL;
    this->info = info;
    level = 0;
}

// skiplist constructor
template <typename T>
Skiplist<T>::Skiplist(T lower_bound, T upper_bound)
{
    this->lower_bound = lower_bound;
    this->upper_bound = upper_bound;
    Node<T> *p = new Node<T>(lower_bound);
    Node<T> *q = new Node<T>(upper_bound);
    p->next = q;
    q->prev = p;
    head = p;
    tail = q;
    head->level = tail->level = 1;
    size = 0;
    height = 1;
}
// skiplist destructor
template <typename T>
Skiplist<T>::~Skiplist<T>()
{
    Node<T> *p = head;
    Node<T> *q, *r;
    while(p!=NULL)
    {
        q = r = p;
        p = p->below;
        while(q!=NULL)
        {
            r = q->next;
            delete q;
            q = r;
        }
    }
}
// skiplist copy-constructor
template <typename T>
Skiplist<T>::Skiplist(const Skiplist<T>& other)
{
    lower_bound = other.lower_bound;
    upper_bound = other.upper_bound;
    Node<T> *p = new Node<T>(lower_bound);
    Node<T> *q = new Node<T>(upper_bound);
    p->next = q;
    q->prev = p;
    head = p;
    tail = q;
    head->level = tail->level = 1;
    size = 0;
    height = 1;
    p = other.head;
    while(p->below!=NULL)
        p = p->below;
    while(p->next->next!=NULL)
    {
        p = p->next;
        Insert(p->info);
    }
}
// skiplist assignment operator
template<typename T>
Skiplist<T>& Skiplist<T>::operator=(const Skiplist<T>& other)
{
    // delete current list
    Node<T> *p = head;
    Node<T> *q, *r;
    while(p!=NULL)
    {
        q = r = p;
        p = p->below;
        while(q!=NULL)
        {
            r = q->next;
            delete q;
            q = r;
        }
    }
    // copy new list
    lower_bound = other.lower_bound;
    upper_bound = other.upper_bound;
    p = new Node<T>(lower_bound);
    q = new Node<T>(upper_bound);
    p->next = q;
    q->prev = p;
    head = p;
    tail = q;
    head->level = tail->level = 1;
    size = 0;
    height = 1;
    p = other.head;
    while(p->below!=NULL)
        p = p->below;
    while(p->next->next!=NULL)
    {
        p = p->next;
        Insert(p->info);
    }
    return *this;
}
// skiplist getters
template <typename T>
int Skiplist<T>::GetSize()
{
    return size;
}

template <typename T>
int Skiplist<T>::GetHeight()
{
    return height;
}

template <typename T>
Node<T>* Skiplist<T>::GetHead()
{
    return head;
}

template <typename T>
Node<T>* Skiplist<T>::GetTail()
{
    return tail;
}
// skiplist operations
template <typename T>
Node<T>* Skiplist<T>::Search(T key)
{
    Node<T> *p;
    Node<T> *q;
    p = q = head;
    do
    {
        while(p->next->info <= key)
            p = p->next;
        if(p->below!=NULL) p = p->below;
        if(p==q) break;
        q = p;
    }while(1);
    return p;
}

template <typename T>
void Skiplist<T>::Insert(T key)
{
    Node<T> *p = Search(key);
    if(p->info == key) return; // keys are unique
    size++;
    Node<T> *q = new Node<T>(key);
    // insert q after p
    q->next = p->next;
    p->next->prev = q;
    q->prev = p;
    p->next = q;
    q->level = 1;
    while(CoinFlip())
    {
        p = new Node<T>(key);
        // insert p above q
        q->above = p;
        p->below = q;
        p->level = q->level + 1;
        if(p->level>height)
        {
			// create new level
            Node<T> *l = new Node<T>(lower_bound);
            Node<T> *r = new Node<T>(upper_bound);
            l->below = head;
            r->below = tail;
            head->above = l;
            tail->above = r;
            head = l;
            tail = r;
            head->next = p;
            p->prev = head;
            tail->prev = p;
            p->next = tail;
            head->level = tail->level = p->level;
            height++;
        }
        else
        {
            q = head;
            while(q->level > p->level)
                q = q->below;
            while(q->next->info <= p->info)
                q = q->next;
            // insert p after q
            p->next = q->next;
            p->prev = q;
            q->next->prev = p;
            q->next = p;
        }
        q = p;
    }

}

template <typename T>
void Skiplist<T>::Delete(T key)
{
    Node<T> *p = Search(key);
    if(p->info!=key) return;
    Node<T> *q;
    while(p!=NULL)
    {
        p->prev->next = p->next;
        p->next->prev = p->prev;
        q = p;
        p = p->above;
        delete q;
    }
    size--;
    while(head->next==tail)
    {
        p = head;
        q = tail;
        p->below->above = NULL;
        q->below->above = NULL;
        head = head->below;
        tail = tail->below;
        delete p;
        delete q;
        height--;
    }
}

template <typename T>
void Skiplist<T>::PrintList()
{
    Node<T> *p = head;
    Node<T> *q = NULL;
    while(p!=NULL)
    {
        q = p;
        while(q!=NULL)
        {
            std::cout<<q->info<<" ";
            q = q->next;
        }
        std::cout<<"\n";
        p = p->below;
    }
    std::cout<<"\n";
}

template class Skiplist<int>;
template class Skiplist<Driver>;