#pragma once
#include <vector>
#include <iostream>
#include <list>


// Adjacency List, if |E| << |V|^2, space O(|V|+|E|)
class GraphL {
  int const vertexCount;
  std::vector<int>* adj  = nullptr;
  std::vector<bool> used; // for DFS
  std::list<int> queue; // for BFS

  void dfsFromVertex(int v) {
    used[v] = true;
    std::cout << v << " ";

    for (auto& i : adj[v]) {
      if (!used[i]) {
        dfsFromVertex(i);
      }
    }
  }

public:

  explicit GraphL(int vertexCount) : vertexCount(vertexCount), used(vertexCount) {
    adj = new std::vector<int>[vertexCount];
  }

  void addEdge(int from, int to) {
    adj[from].push_back(to);
    adj[to].push_back(from);
  }

  void dfs() {
    std::cout << "Start DFS: " << std::endl;
    std::fill(used.begin(), used.end(), false);

    for (int i = 0; i < vertexCount; ++i) {
      if (!used[i]) {
        dfsFromVertex(i);
      }
    }
    std::cout << std::endl;
  }

  void bfs(int start = 0) {
    std::cout << "Start BFS: " << std::endl;
    std::fill(used.begin(), used.end(), false);

    used[start] = true;
    queue.push_back(start);

    while (!queue.empty()) {
      int v = queue.front();
      std::cout << v << " ";
      queue.pop_front();

      for (auto& i : adj[v]) {
        if (!used[i]) {
          used[i] = true;
          queue.push_back(i);
        }
      }
    }
    std::cout << std::endl;
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