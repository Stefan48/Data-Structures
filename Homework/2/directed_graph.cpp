#include "directed_graph.h"


template <typename T>
DirectedGraph<T>::DirectedGraph(int cnt_nodes)
{
    this->cnt_nodes = cnt_nodes;
    nodes = new Node<T>[cnt_nodes];
}

template <typename T>
DirectedGraph<T>::~DirectedGraph()
{
    delete[] nodes;
}

template <typename T>
int DirectedGraph<T>::size()
{
    return cnt_nodes;
}

template <typename T>
Node<T>& DirectedGraph<T>::operator[](int index)
{
    return nodes[index];
}

template <typename T>
bool DirectedGraph<T>::hasEdge(int source, int dest)
{
    for(auto it = nodes[source].neighbors.begin(); it != nodes[source].neighbors.end(); ++it)
        if(*it == dest) return true;
    return false;
}

template <typename T>
void DirectedGraph<T>::addEdge(int source, int dest)
{
    if(!hasEdge(source, dest))
        nodes[source].neighbors.push_back(dest);
}

template <typename T>
void DirectedGraph<T>::removeEdge(int source, int dest)
{
    nodes[source].neighbors.remove(dest);
}

template <typename T>
int DirectedGraph<T>::distance(int source, int dest)
{
    bool mark[cnt_nodes];
    for(int i = 0; i < cnt_nodes; ++i) mark[i] = false;
    int dist[cnt_nodes];
    for(int i = 0; i < cnt_nodes; ++i) dist[i] = -1;
    std::queue<int> Q;
    int n;
    dist[source] = 0;
    mark[source] = true;
    Q.push(source);
    while(!Q.empty())
    {
        n = Q.front();
        Q.pop();
        for(auto it = nodes[n].neighbors.begin(); it != nodes[n].neighbors.end(); ++it)
            if(!mark[*it])
            {
                mark[*it] = true;
                dist[*it] = dist[n] + 1;
                Q.push(*it);
            }
    }
    return dist[dest];
}

template class DirectedGraph<std::string>;