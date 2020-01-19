#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include "list.h"
#include "vertex.h"

// Adjacency List, if |E| << |V|^2, space O(|V|+|E|)
class GraphL {
  int const vertexCount;
  std::vector<Vertex>* adj  = nullptr;
  std::vector<bool> used; // for DFS
  List<int> list; // for BFS

  void dfsFromVertex(int v) {
    used[v] = true;
    std::cout << v << " ";

    for (auto& vertex : adj[v]) {
      int i = vertex.v;
      if (!used[i]) {
        dfsFromVertex(i);
      }
    }
  }

public:

  explicit GraphL(int vertexCount) : vertexCount(vertexCount), used(vertexCount) {
    adj = new std::vector<Vertex>[vertexCount];
  }

  void addEdge(int from, int to, int w = 1) {
    adj[from].emplace_back(Vertex(to, w));
    adj[to].emplace_back(Vertex(from, w));
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
    list.push_back(start);

    while (!list.isEmpty()) {
      int v = list.front();
      std::cout << v << " ";
      list.pop_front();

      for (auto& vertex : adj[v]) {
        int i = vertex.v;
        if (!used[i]) {
          used[i] = true;
          list.push_back(i);
        }
      }
    }
    std::cout << std::endl;
  }

  void kruskalMST() {
    std::vector<Edge> edges;
    for (int i = 0; i < vertexCount; ++i) {
      for (auto& vertex : adj[i]) {
        edges.emplace_back(Edge(i, vertex.v, vertex.w));
      }
    }

    std::vector<Edge> result;
    std::sort(edges.begin(), edges.end());

    DsuTreeSizeHeuristic dsu(vertexCount);

    int minCost = 0;
    for (auto& edge : edges) {
      int u_set = dsu.find_set(edge.from);
      int v_set = dsu.find_set(edge.to);
      if (u_set != v_set) {
        result.push_back(edge);
        dsu.union_sets(u_set, v_set);
        minCost += edge.w;
      }
    }

    printMST(minCost, result);
  }

  void print() {
    for (int v = 0; v < vertexCount; ++v) {
      std::cout << "Adjacency list of vertex " << v << "\n head ";
      for (auto& vertex : adj[v]) {
        std::cout << " -> " << vertex.v;
      }
      std::cout << std::endl;
    }
  }
};