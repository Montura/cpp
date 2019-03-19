#pragma once

struct Widget {
  bool operator<(Widget const& rhs) { return false; }
};
