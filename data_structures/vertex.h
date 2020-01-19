#pragma once

struct Vertex {
  int v = 0, w = 1;

  explicit Vertex(int v, int weight = 1) : v(v), w(weight) {}
};

