#pragma once

struct Widget {
  Widget() {}

  Widget(int i) {}

  bool operator<(Widget const& rhs) { return false; }
};
