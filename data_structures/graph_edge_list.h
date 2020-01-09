#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "dsu.h"
#include "Edge.h"

class GraphE {
  int vertexCount = 0;
  std::vector<Edge> edges {};

public:
  explicit GraphE() {}

  void addEdge(int from, int to, int w = 1) {
    vertexCount = std::max(vertexCount, std::max(from, to)) + 1; // v index start from 0
    edges.emplace_back(Edge(from, to, w));
  }

  // O (E*logE)
  void kruskalMST() {
    std::sort(edges.begin(), edges.end());

    std::vector<Edge> result;
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

};