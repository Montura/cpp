#pragma once

#include <vector>
#include <iostream>
#include <list>
#include "dsu.h"
#include "Edge.h"

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

  void addEdge(int from, int to, int w = 1) {
    adj[from][to] = w;
    adj[to][from] = w;
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

  // O (E*logE)
  void kruskalMST() {
    int minCost = 0;
    std::vector<Edge> result;
    DsuTreeSizeHeuristic dsu(vertexCount);

    int edgeCount = 0;
    while (edgeCount < vertexCount - 1) {
      int minWeight = INT32_MAX, from = -1, to = -1;
      for (int i = 0; i < vertexCount; ++i) {
        for (int j = 0; j < vertexCount; ++j) {
          int u_set = dsu.find_set(i);
          int v_set = dsu.find_set(j);

          int edgeWeight = adj[i][j];
          if (u_set != v_set && edgeWeight > 0 && edgeWeight < minWeight) {
            from = i;
            to = j;
            minWeight = edgeWeight;
          }
        }
      }

      edgeCount++;
      result.emplace_back(Edge(from, to, minWeight));
      dsu.union_sets(from, to);
      minCost += minWeight;
    }

    printMST(minCost, result);
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
