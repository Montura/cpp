#pragma once

// default impl
class DSU {
protected:
  std::vector<int> parent;

public:
  explicit DSU(int size) : parent(size) {
    for (int i = 0; i < size; ++i) {
      parent[i] = i;
    }
  }

  virtual void union_sets(int x, int y) {
    int xRoot = find_set(x);
    int yRoot = find_set(y);

    if (xRoot != yRoot) {
      parent[x] = yRoot;
    }
  }

  virtual int find_set(int x) {
    while (parent[x] != x) {
      x = parent[x];
    }
    return x;
  }

  bool isConnected(int x, int y) {
    return find_set(x) == find_set(y);
  }
};

class DsuTreeSizeHeuristic : public DSU {
  std::vector<int> rank; // or rank, rank [i] = 0;

public:
  explicit DsuTreeSizeHeuristic(int n) : DSU(n), rank(n) {
    for (int i = 0; i < n; ++i) {
      rank[i] = 1;
    }
  }

  void union_sets(int x, int y) override {
    int rootX = find_set(x);
    int rootY = find_set(y);
    if (rootX != rootY) {
      if (rank[rootX] < rank[rootY]) {
        std::swap(rootX, rootY);
      }
      parent[rootY] = rootX;
      rank[rootX] += rank[rootY];
      // for rank heuristic
      // if (rank[x] == rank[y]) {
      //   rank[x] ++;
      // }
    }
  }

  int getSize(int x) {
    return rank[find_set(x)];
  }
};


class DsuPathCompression : DSU {
  // Now std::vector<int> parent - compressed array of ancestors

  explicit DsuPathCompression(int n) : DSU(n) {}

  int find_set(int x) override {
    if (x != parent[x]) {
      // update parent[v] for each v in path to root(x)
      parent[x] = find_set(parent[x]);
    }
    return x;
  }
};


// DSU + Rank(Size) heuristic + PathCompression => query O(log* n) is almost O(1)