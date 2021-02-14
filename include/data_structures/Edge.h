#pragma once

struct Edge {
  int from = 0;
  int to = 0;
  int w = 1;

  Edge(int from, int to, int weight = 1) : from(from), to(to), w(weight) {}
};

bool operator<(const Edge& a, const Edge& b) {
  return a.w < b.w;
}

std::ostream& operator<<(std::ostream& os, const Edge& e) {
  os << e.from << " -> " << e.to << ", w = " << e.w;
  return os;
}


void printMST(int minCost, const std::vector<Edge> &result) {
  std::cout << "Edges of minimal spanning tree are: " << std::endl;
  for (auto& e : result) {
    std::cout << e << std::endl;
  }
  std::cout << "Weight of MST is: " << minCost << std::endl;
}

