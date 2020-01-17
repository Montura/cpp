#include "stack.h"

using namespace std;

void test_stack() {
  StackM<int, Stat> s(10);

  cout << "s.isFull(): " << s.isFull() << endl;
  cout << "s.isEmpty(): " << s.isEmpty() << endl;
  cout << "Pop : " << s.pop() << std::endl;

  s.push(1);
  cout << "s.isFull(): " << s.isFull() << endl;
  cout << "s.isEmpty(): " << s.isEmpty() << endl;
  cout << "Pop : " << s.pop() << std::endl;

  s.push(0);
  s.push(1);
  s.push(2);
  s.push(3);
  s.push(4);
  cout << "s.Min(): " << s.getStat() << endl;
  s.push(5);
  cout << "s.Min(): " << s.getStat() << endl;
  s.push(6);
  s.push(-1);
  cout << "s.Min(): " << s.getStat() << endl;
  s.push(8);
  s.push(9);
  cout << "s.isFull(): " << s.isFull() << endl;
  cout << "s.isEmpty(): " << s.isEmpty() << endl;
  cout << "Pop : " << s.pop() << std::endl;
  s.push(11);
  cout << "Pop : " << s.pop() << std::endl;
  s.push(12);
  s.push(13);
  cout << "Pop : " << s.pop() << std::endl;

}