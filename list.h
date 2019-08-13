//
// Created by elmon on 11.08.2019.
//

#ifndef CPP_LIST_H
#define CPP_LIST_H

struct Node {
  Node* next = nullptr;
  int const data = 0;

  explicit Node(int data = 0) : next(nullptr), data(data) {}

  void insertAfter(int newData) {
    Node* newNode = new Node(newData);
    newNode->next = this->next;
    this->next = newNode;
  }

};

struct List {
  Node* head = nullptr;
  Node* tail = nullptr;
  int size = 0;

  explicit List(Node* head = nullptr, Node* tail = nullptr) : head(head), tail(head)  {}

  void push_front(int newData) {
    Node* newNode = new Node(newData);
    newNode->next = head;
    tail = head;
    head = newNode;
  }

  void push_back(int newData) {
    Node* newNode = new Node(newData);
    Node* current = head;
    while (current->next != nullptr) {
      current = current->next;
    }
    current->next = newNode;
    tail = newNode;
  }

  void pop_front() {
    if (head != nullptr) {
      Node* newHead = head->next;
      delete head;
      head = newHead;
    }
  }

  void pop_back() {
    Node* current = head;
    Node* next = head->next;
    while (next->next != nullptr) {
      current = current->next;
      next = current->next;
    }
    tail = current;
    tail->next = nullptr;
    delete next;
  }

  void print() {
    Node* curr = head;
    while(curr != nullptr) {
      std::cout << curr->data << " ";
      curr = curr->next;
    }
    std::cout << std::endl;
  }

};


#endif //CPP_LIST_H
