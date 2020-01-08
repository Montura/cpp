#include <vector>
#include <iostream>
#include "dsu.h"

// Sample Input
// 3 6
// Q 1
// M 1 2
// Q 2
// M 2 3
// Q 3
// Q 2
// Sample Output
// 1
// 2
// 3
// 3

void test_dsu_size_stat() {
  int n = 0, q = 0;
  DsuTreeSizeHeuristic d(n);
  std::cin >> n >> q;
  DsuTreeSizeHeuristic dsu(n);
  char s = 0;
  int x = 0, y = 0;
  while (q--) {
    std::cin >> s;
    switch (s) {
      case 'Q':
        std::cin >> x;
        std::cout << dsu.getSize(x) << std::endl;
        break;
      case 'M':
        std::cin >> x >> y;
        dsu.union_sets(x, y);
        break;
      default:
        break;
    }
  }
}