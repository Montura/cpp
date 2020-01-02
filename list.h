//
// Created by elmon on 11.08.2019.
//

#ifndef CPP_LIST_H
#define CPP_LIST_H
#include <cstdint>

struct Node {
  Node* next = nullptr;
  int32_t const data = 0;

  explicit Node(int32_t data = 0) : next(nullptr), data(data) {}
};

class List {
  Node* head = nullptr;
  Node* tail = nullptr;
  int size = 0;

public:
  explicit List() = default;

  void pushFront(int32_t newData) {
    Node* newNode = new Node(newData);
    newNode->next = head;
    head = newNode;
    if (tail == nullptr) {
      tail = newNode;
    }
    ++size;
  }

  bool insertAfter(Node* node, int32_t newData) {
    Node* current = head;
    while (current != node) {
      current = current->next;
    }
    if (current != nullptr) {
      Node* newNode = new Node(newData);
      if (current == tail) {
        tail = newNode;
      }
      newNode->next = current->next;
      current->next = newNode;
      ++size;
    }

    return false;
  }

  void pushBack(int32_t newData) {
    Node* newNode = new Node(newData);
    Node* current = head;
    while (current->next != nullptr) {
      current = current->next;
    }
    current->next = newNode;
    tail = newNode;
    ++size;
  }

  void popFront() {
    if (head != nullptr) {
      Node* newHead = head->next;
      delete head;
      head = newHead;
      if (head == nullptr) {
        tail = nullptr;
      }
      --size;
    }
  }

  void popBack() {
    if (head != nullptr) {
      Node* curr = head;
      Node* next = curr->next;
      while (next != nullptr && next->next != nullptr) {
        curr = next;
        next = curr->next;
      }
      if (next != nullptr) {
        tail = curr;
        tail->next = nullptr;
        delete next;
      } else {
        delete curr;
        head = tail = nullptr;
      }

      --size;
    }
  }

  void print() {
    Node* curr = head;
    while(curr != nullptr) {
      std::cout << curr->data << " ";
      curr = curr->next;
    }
    std::cout << ", size is " << size;
    std::cout << ", head is " << head << ": " << ((head != nullptr) ? head->data : -1);
    std::cout << ", tail is " << tail << ": " << ((tail != nullptr) ? tail->data : -1) << std::endl;
    std::cout << std::endl;
  }

  Node* begin() {
    return head;
  }
};


#endif //CPP_LIST_H
