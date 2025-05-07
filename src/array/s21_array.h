#ifndef SRC_S21_ARRAY__HPP
#define SRC_S21_ARRAY__HPP

#include <algorithm>
#include <memory>
#include <stdexcept>

namespace s21 {

template <typename T, size_t Size>
class array {
 private:
  std::unique_ptr<T[]> values_;

 public:
  class iterator;
  class const_iterator;

  array() : values_(std::make_unique<T[]>(Size)) {}

  array(std::initializer_list<T> const& items)
      : values_(std::make_unique<T[]>(Size)) {
    size_t i = 0;
    for (auto item = items.begin(); item != items.end(); ++item)
      values_[i++] = *item;
    for (; i < Size; ++i) values_[i] = T();
  }

  array(const array& a) : values_(std::make_unique<T[]>(Size)) {
    for (size_t i = 0; i < Size; ++i) values_[i] = a.values_[i];
  }

  array(array&& a) noexcept : values_(std::move(a.values_)) {}

  ~array() = default;

  array& operator=(const array& a) {
    if (this != &a) {
      values_ = std::make_unique<T[]>(Size);
      for (size_t i = 0; i < Size; ++i) values_[i] = a.values_[i];
    }
    return *this;
  }

  array& operator=(array&& a) noexcept {
    if (this != &a) values_ = std::move(a.values_);
    return *this;
  }

  T& at(size_t pos) {
    if (pos >= Size) throw std::out_of_range("Index out of range");
    return values_[pos];
  }

  T& operator[](size_t pos) { return values_[pos]; }

  const T& front() const { return values_[0]; }

  const T& back() const { return values_[Size - 1]; }

  iterator data() { return begin(); }

  iterator begin() { return iterator(values_.get()); }
  iterator end() { return iterator(values_.get() + Size); }

  const_iterator begin() const { return const_iterator(values_.get()); }
  const_iterator end() const { return const_iterator(values_.get() + Size); }

  bool empty() const { return Size == 0; }

  size_t size() const { return Size; }

  size_t max_size() const { return Size; }

  void swap(array& other) {
    for (size_t i = 0; i < Size; ++i) std::swap(values_[i], other.values_[i]);
  }

  void fill(const T& value) {
    for (size_t i = 0; i < Size; ++i) {
      values_[i] = value;
    }
  }

  bool operator==(const array& other) const {
    return std::equal(values_.get(), values_.get() + Size, other.values_.get());
  }

  bool operator!=(const array& other) const { return !(*this == other); }

  class iterator {
   public:
    explicit iterator(T* ptr) : ptr_(ptr) {}

    T& operator*() const { return *ptr_; }
    iterator& operator++() {
      ++ptr_;
      return *this;
    }
    iterator operator++(int) {
      iterator temp = *this;
      ++(*this);
      return temp;
    }
    iterator& operator--() {
      --ptr_;
      return *this;
    }
    iterator operator--(int) {
      iterator temp = *this;
      --(*this);
      return temp;
    }
    bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
    bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }

   private:
    T* ptr_;
  };

  class const_iterator {
   public:
    explicit const_iterator(const T* ptr) : ptr_(ptr) {}

    const T& operator*() const { return *ptr_; }
    const_iterator& operator++() {
      ++ptr_;
      return *this;
    }
    const_iterator operator++(int) {
      const_iterator temp = *this;
      ++(*this);
      return temp;
    }
    const_iterator& operator--() {
      --ptr_;
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator temp = *this;
      --(*this);
      return temp;
    }
    bool operator==(const const_iterator& other) const {
      return ptr_ == other.ptr_;
    }
    bool operator!=(const const_iterator& other) const {
      return ptr_ != other.ptr_;
    }

   private:
    const T* ptr_;
  };
};

}  // namespace s21

#endif  // SRC_S21_ARRAY__HPP
