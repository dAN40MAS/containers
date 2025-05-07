#ifndef S21_LIST_H
#define S21_LIST_H

#include <cstddef>
#include <stdexcept>

namespace s21 {

template <typename T>
class list {
 private:
  struct Node {
    T data;
    Node* prev;
    Node* next;
    Node(const T& value);
  };

  size_t size_;
  Node* head_;
  Node* tail_;

 public:
  list();
  list(const list& other);
  list(list&& other);
  ~list();

  list& operator=(const list& other);
  list& operator=(list&& other);

  size_t size() const;
  bool empty() const;

  void push_back(const T& value);
  void push_front(const T& value);
  void pop_back();
  void pop_front();
  void clear();

  class iterator {
   private:
    Node* current_;
    const list<T>* parent_list_;

   public:
    iterator(Node* node, const list<T>* parent);
    iterator& operator++();
    iterator& operator--();
    T& operator*();
    bool operator==(const iterator& other) const;
    bool operator!=(const iterator& other) const;
  };

  iterator begin();
  iterator end();

  T& front();
  T& back();
};

template <typename T>
list<T>::Node::Node(const T& value)
    : data(value), prev(nullptr), next(nullptr) {}

template <typename T>
list<T>::list() : size_(0), head_(nullptr), tail_(nullptr) {}

template <typename T>
list<T>::list(const list<T>& other) : list() {
  for (Node* node = other.head_; node != nullptr; node = node->next) {
    push_back(node->data);
  }
}

template <typename T>
list<T>::list(list<T>&& other)
    : size_(other.size_), head_(other.head_), tail_(other.tail_) {
  other.size_ = 0;
  other.head_ = nullptr;
  other.tail_ = nullptr;
}

template <typename T>
list<T>::~list() {
  clear();
}

template <typename T>
list<T>& list<T>::operator=(const list<T>& other) {
  if (this != &other) {
    clear();
    for (Node* node = other.head_; node != nullptr; node = node->next) {
      push_back(node->data);
    }
  }
  return *this;
}

template <typename T>
list<T>& list<T>::operator=(list<T>&& other) {
  if (this != &other) {
    clear();
    size_ = other.size_;
    head_ = other.head_;
    tail_ = other.tail_;
    other.size_ = 0;
    other.head_ = nullptr;
    other.tail_ = nullptr;
  }
  return *this;
}

template <typename T>
size_t list<T>::size() const {
  return size_;
}

template <typename T>
bool list<T>::empty() const {
  return size_ == 0;
}

template <typename T>
void list<T>::push_back(const T& value) {
  Node* new_node = new Node(value);
  if (tail_) {
    tail_->next = new_node;
    new_node->prev = tail_;
    tail_ = new_node;
  } else {
    head_ = tail_ = new_node;
  }
  ++size_;
}

template <typename T>
void list<T>::push_front(const T& value) {
  Node* new_node = new Node(value);
  if (head_) {
    head_->prev = new_node;
    new_node->next = head_;
    head_ = new_node;
  } else {
    head_ = tail_ = new_node;
  }
  ++size_;
}

template <typename T>
void list<T>::pop_back() {
  if (tail_) {
    Node* temp = tail_;
    tail_ = tail_->prev;
    if (tail_) {
      tail_->next = nullptr;
    } else {
      head_ = nullptr;
    }
    delete temp;
    --size_;
  }
}

template <typename T>
void list<T>::pop_front() {
  if (head_) {
    Node* temp = head_;
    head_ = head_->next;
    if (head_) {
      head_->prev = nullptr;
    } else {
      tail_ = nullptr;
    }
    delete temp;
    --size_;
  }
}

template <typename T>
void list<T>::clear() {
  while (!empty()) {
    pop_front();
  }
}

template <typename T>
list<T>::iterator::iterator(Node* node, const list<T>* parent)
    : current_(node), parent_list_(parent) {}

template <typename T>
typename list<T>::iterator& list<T>::iterator::operator++() {
  if (current_) {
    current_ = current_->next;
  }
  return *this;
}

template <typename T>
typename list<T>::iterator& list<T>::iterator::operator--() {
  if (current_) {
    current_ = current_->prev;
  } else {
    // Если current_ == nullptr (end()), устанавливаем на tail_
    current_ = parent_list_->tail_;
  }
  return *this;
}

template <typename T>
T& list<T>::iterator::operator*() {
  if (current_) {
    return current_->data;
  } else {
    throw std::runtime_error("Dereferencing end() iterator");
  }
}

template <typename T>
bool list<T>::iterator::operator==(const iterator& other) const {
  return current_ == other.current_;
}

template <typename T>
bool list<T>::iterator::operator!=(const iterator& other) const {
  return current_ != other.current_;
}

template <typename T>
typename list<T>::iterator list<T>::begin() {
  return iterator(head_, this);
}

template <typename T>
typename list<T>::iterator list<T>::end() {
  return iterator(nullptr, this);
}

template <typename T>
T& list<T>::front() {
  if (head_) {
    return head_->data;
  } else {
    throw std::runtime_error("List is empty");
  }
}

template <typename T>
T& list<T>::back() {
  if (tail_) {
    return tail_->data;
  } else {
    throw std::runtime_error("List is empty");
  }
}

}  // namespace s21

#endif  // S21_LIST_H
