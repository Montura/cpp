#include <iostream>
#include "queue.h"

void test_queue() {
  Queue<int> q;
  q.enqueue(1);
  q.enqueue(2);
  q.enqueue(3);

  std::cout << q.dequeue() << '\n';
  std::cout << q.dequeue() << '\n';
  std::cout << q.dequeue() << '\n';

  Queue<int> q2;
  q2.enqueue(10);
  q2.enqueue(20);
  q2.enqueue(30);
  q2.enqueue(40);
  q2.enqueue(50);
  q2.enqueue(60);
  q2.enqueue(70);
  q2.enqueue(80);
  q2.enqueue(90);
  q2.enqueue(100);

  int k = 5;
  q2.reverseQueueFirstKElements(k);
  std ::cout << q2;
}