#pragma once

#include <vector>
#include <iostream>
#include <list>

// Adjacency Matrix, space O(|V|^2)
class GraphM {
  int const vertexCount;
  std::vector<int>* adj  = nullptr;
  std::vector<bool> used; // for DFS
  std::list<int> queue; // for BFS

  void dfsFromVertex(int v) {
    used[v] = true;
    std::cout << v << " ";

    for (auto & edge : adj[v]) {
      if (edge && !used[v]) {
        dfsFromVertex(v);
      }
    }
  }

public:
  explicit GraphM(int vertexCount) : vertexCount(vertexCount), used(vertexCount) {
    adj = new std::vector<int> [vertexCount];
    for (int i = 0; i < vertexCount; ++i) {
      adj[i].resize(vertexCount);
    }
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

  void bfs(int v = 0) {
    std::cout << "Start BFS: " << std::endl;
    std::fill(used.begin(), used.end(), false);

    used[v] = true;
    queue.push_back(v);

    while (!queue.empty()) {
      int curr = queue.front();
      std::cout << curr << " ";
      queue.pop_front();

      for (int i = 0; i < vertexCount; ++i) {
        bool hasEdge = adj[curr][i] > 0;
        if (hasEdge && !used[i]) {
          used[i] = true;
          queue.push_back(i);
        }
      }
    }
    std::cout << std::endl;
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
};
