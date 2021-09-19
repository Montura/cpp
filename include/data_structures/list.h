#pragma once
#include "node.h"

template <typename T>
class List {
  Node<T>* head_ = nullptr;
  Node<T>* tail_ = nullptr;
  int size_ = 0;

public:
  explicit List() = default;

  bool isEmpty() const;
  int size() const;
  void push_front(T newData);
  void push_back(T newData);
  void pop_front();
  void pop_back();

  T front();
  const T& front() const;

  void reverse_list();

  template <typename U>
  friend std::ostream& operator<<(std::ostream& out, const List<U>&);
};

template<typename T>
bool List<T>::isEmpty() const {
  return size_ == 0;
}

template<typename T>
int List<T>::size() const {
  return size_;
}

template<typename T>
void List<T>::push_front(T newData) {
  auto* newNode = new Node<T>(newData);
  if (head_ == nullptr) {
    head_ = newNode;
    tail_ = head_;
  } else {
    newNode->next = head_;
    head_ = newNode;
  }
  ++size_;
}

template<typename T>
void List<T>::push_back(T newData) {
  auto* newNode = new Node<T>(newData);
  if (head_ == nullptr) {
    head_ = newNode;
    tail_ = head_;
  } else {
    tail_->next = newNode;
    tail_ = newNode;
  }
  ++size_;
}

template<typename T>
void List<T>::pop_front() {
  if (isEmpty()) return;

  auto newHead = head_->next;
  delete head_;
  head_ = newHead;
  if (head_ == nullptr) {
    tail_ = nullptr;
  }
  --size_;
}

template<typename T>
void List<T>::pop_back() {
  if (isEmpty()) return;

  if (head_ == tail_) {
    delete head_;
    head_ = tail_ = nullptr;
  }
  auto* curr = head_;
  while (curr->next != tail_) {
    curr = curr->next;
  }
  tail_ = curr;
  delete tail_->next;
  tail_->next = nullptr;
  --size_;
}

template<typename T>
T List<T>::front() {
  if (size_ == 0) {
    std::cout << "List is empty!" << std::endl;
    return INT_MAX;
  }
  return head_->data;
}

template<typename T>
const T& List<T>::front() const {
  if (size_ == 0) {
    std::cout << "List is empty!" << std::endl;
    return INT_MAX;
  }
  return head_->data;
}

template<typename T>
void List<T>::reverse_list() {
  if (isEmpty()) return;

  auto curr = head_;
  Node<T>* prev = nullptr;
  Node<T>* next = nullptr;

  while (curr != nullptr) {
    next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
  }
  tail_ = head_;
  head_ = prev;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const List<T>& l) {
  auto* curr = l.head_;
  while (curr != nullptr) {
    out << curr->data << " ";
    curr = curr->next;
  }
  out << ", size_ is " << l.size();
  out << ", head_ is " << l.head_ << ": " << ((l.head_ != nullptr) ? l.head_->data : -1);
  out << ", tail_ is " << l.tail_ << ": " << ((l.tail_ != nullptr) ? l.tail_->data : -1) << std::endl;
  out << std::endl;

  return out;
}