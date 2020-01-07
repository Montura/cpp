#pragma once
#include <vector>
#include <iostream>

// Adjacency Matrix, space O(|V|^2)
class Matrix {
  std::vector<int>* adj;

public:
  int const vertexCount;

  explicit Matrix(int vertexCount) : vertexCount(vertexCount) {
    adj = new std::vector<int> [vertexCount];
    for (int i = 0; i < vertexCount; ++i) {
      adj[i].resize(vertexCount);
    }
  }

  ~Matrix() {
    delete[] adj;
  }

  void addEdge(int from, int to) {
    adj[from][to] = 1;
    adj[to][from] = 1;
  }

  void print() {
    std::cout << "Adjacency matrix: \n   ";
    for (int i = 0; i < vertexCount; ++i) {
      std::cout << i << "_";
    }
    std::cout << std::endl;
    for (int i = 0; i < vertexCount; ++i) {
      std::cout << i << "| ";
      for (int j = 0; j < vertexCount; ++j) {
        std::cout << adj[i][j] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }

  std::vector<int>& operator[](int i) {
    return adj[i];
  }
};

// Adjacency List, if |E| << |V|^2, space O(|V|+|E|)
class List {
  std::vector<int>* adj;

public:
  int const vertexCount;

  explicit List(int vertexCount) : vertexCount(vertexCount) {
    adj = new std::vector<int>[vertexCount];
  }

  ~List() {
    delete[] adj;
  }

  void addEdge(int from, int to) {
    adj[from].push_back(to);
    adj[to].push_back(from);
  }

  std::vector<int>& operator[](int i) {
    return adj[i];
  }

  void print() {
    for (int v = 0; v < vertexCount; ++v) {
      std::cout << "Adjacency list of vertex " << v << "\n head ";
      for (auto& w : adj[v]) {
        std::cout << " -> " << w;
      }
      std::cout << std::endl;
    }
  }
};

template <class T>
class Graph {
  T graph;
  std::vector<bool> used; // for DFS

  void dfsFromVertex(int v) {
    used[v] = true;
    std::cout << v << " ";
    
    for (auto& adjV : graph[v]) {
      if (!used[adjV]) {
        dfsFromVertex(adjV);
      }
    }
  }
  
public:
  explicit Graph(int const vertexCount) : graph(T(vertexCount)), used(vertexCount, false) {}

  void dfs() {
    std::cout << "Start DFS: " << std::endl;
    for (int i = 0; i < graph.vertexCount; ++i) {
      if (!used[i]) {
        dfsFromVertex(i);
      }
    }
    std::cout << std::endl;
  }

  void addEdge(int from, int to) {
    graph.addEdge(from, to);
  }

  void print() {
    graph.print();
  }
};


void test_graph_impl() {
  int size = 5;
  Graph<List> g(size);

  g.addEdge(0, 1);
  g.addEdge(0, 4);
  g.addEdge(1, 2);
  g.addEdge(1, 3);
  g.addEdge(1, 4);
  g.addEdge(2, 3);
  g.addEdge(3, 4);
  g.print();

  g.dfs();

  Graph<Matrix> gM(size);
  gM.addEdge(0, 1);
  gM.addEdge(0, 4);
  gM.addEdge(1, 2);
  gM.addEdge(1, 3);
  gM.addEdge(1, 4);
  gM.addEdge(2, 3);
  gM.addEdge(3, 4);

  gM.print();
  gM.dfs();
}