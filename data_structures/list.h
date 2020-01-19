#pragma once
#include "node.h"

template <typename T>
class List {
  Node<T>* head = nullptr;
  Node<T>* tail = nullptr;
  int size = 0;

public:
  explicit List() = default;

  bool isEmpty() const {
    return size == 0;
  }

  void push_front(T newData) {
    auto* newNode = new Node<T>(newData);
    newNode->next = head;
    head = newNode;
    if (tail == nullptr) {
      tail = newNode;
    }
    ++size;
  }

  void push_back(T newData) {
    auto* newNode = new Node<T>(newData);
    if (head == nullptr) {
      head = newNode;
    } else {
      auto* current = head;
      while (current->next != nullptr) {
        current = current->next;
      }
      current->next = newNode;
    }
    tail = newNode;
    ++size;
  }

  void pop_front() {
    if (head != nullptr) {
      auto* newHead = head->next;
      delete head;
      head = newHead;
      if (head == nullptr) {
        tail = nullptr;
      }
      --size;
    }
  }

  void pop_back() {
    if (head != nullptr) {
      auto* curr = head;
      auto* next = curr->next;
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
    auto* curr = head;
    while(curr != nullptr) {
      std::cout << curr->data << " ";
      curr = curr->next;
    }
    std::cout << ", size is " << size;
    std::cout << ", head is " << head << ": " << ((head != nullptr) ? head->data : -1);
    std::cout << ", tail is " << tail << ": " << ((tail != nullptr) ? tail->data : -1) << std::endl;
    std::cout << std::endl;
  }

  T front() {
    if (size == 0) {
      std::cout << "List is empty!" << std::endl;
      return INT_MAX;
    }
    return head->data;
  }

  const T& front() const {
    if (size == 0) {
      std::cout << "List is empty!" << std::endl;
      return INT_MAX;
    }
    return head->data;
  }

  void reverse_list() {
    if (head != nullptr) {
      auto* curr = head;
      Node<T>* prev = nullptr;
      Node<T>* next = nullptr;

      while (curr != nullptr) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
      }

      tail = head;
      head =  prev;
    }
  }
};