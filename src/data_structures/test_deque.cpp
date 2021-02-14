#include <iostream>
#include "data_structures/deque.h"
using namespace std;

void test_deque() {
  Deque<int> dq(5);

  cout << "Push back 5 \n";
  dq.push_back(5);

  cout << "Push back 10 \n";
  dq.push_back(10);

  cout << "Pop back last element " << dq.pop_back() << endl;
  cout << "After delete last element become " << dq.last() << endl;

  cout << "Push front 15 \n";
  dq.push_front(15);

  cout << "Pop front element " << dq.pop_front() << endl;
  cout << "After delete front element become " << dq.front() << endl;
}