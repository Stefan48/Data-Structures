#ifndef __DIRECTED_GRAPH_H__
#define __DIRECTED_GRAPH_H__

#include <list>
#include <queue>
#include <string>

template <typename T>
struct Node
{
    std::list<int> neighbors;
    T info;
};

template <typename T>
class DirectedGraph
{
private:
    int cnt_nodes;
    Node<T> *nodes;

public:
    DirectedGraph(int cnt_nodes);
    ~DirectedGraph();

    int size();
    Node<T>& operator[] (int);

    bool hasEdge(int source, int dest);
    void addEdge(int source, int dest);
    void removeEdge(int source, int dest);

    int distance(int source, int dest);
};

#endif // __DIRECTED_GRAPH_H__