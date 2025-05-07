#ifndef SRC_S21_VECTOR__H
#define SRC_S21_VECTOR__H

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <stdexcept>

namespace s21 {

template <typename T, typename Alloc = std::allocator<T>>
class Vector {
  using alloc_traits = std::allocator_traits<Alloc>;

  // Define iterator and const_iterator types
  using iterator = T*;
  using const_iterator = const T*;

 private:
  size_t size_;  ///< Текущий размер вектора (число элементов).
  size_t capacity_;  ///< Емкость вектора (выделенная память).
  T* data_;  ///< Указатель на динамически выделенный массив.
  Alloc allocator_;  ///< Экземпляр аллокатора для управления памятью.

  void copyFrom(T* src);
  void initInsert(size_t len);
  void addBackValue(T* inputData, const T& value);

  void allocateMemory_(size_t capacity);

  void deallocateMemory_();

  void destroyElements_();

  void copyElements_(const T* src);

 public:
  Vector();

  explicit Vector(size_t size);

  Vector(const std::initializer_list<T>& list);

  Vector(const Vector& other);

  Vector(Vector&& other) noexcept(
      alloc_traits::propagate_on_container_swap::value);

  ~Vector();

  Vector& operator=(const Vector& other) noexcept;

  Vector& operator=(Vector&& other) noexcept;

  Vector& operator=(std::initializer_list<T> const& list);

  T& at(size_t index);

  const T& at(size_t index) const;

  T& operator[](size_t index);

  const T& operator[](size_t index) const;

  T& front();

  const T& front() const;

  T& back();

  const T& back() const;

  iterator data() noexcept;

  const_iterator data() const noexcept;

  iterator begin() noexcept;

  iterator end() noexcept;

  const_iterator begin() const noexcept;

  const_iterator end() const noexcept;

  size_t size() const noexcept;

  size_t capacity() const noexcept;

  void reserve(size_t new_capacity);

  void shrink_to_fit();

  void clear();

  void push_back(const T& value);

  void pop_back();

  iterator insert(const_iterator pos, const T& value);

  iterator insert(const_iterator pos, T&& value);

  iterator insert(const_iterator pos, size_t count, const T& value);

  iterator erase(iterator pos);

  void swap(Vector& other) noexcept(
      alloc_traits::propagate_on_container_swap::value);

  bool empty() const noexcept;

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args);

  template <typename... Args>
  void insert_many_back(Args&&... args);

  // Iterator operations
  iterator operator++(int);  // Post-increment
  iterator& operator++();    // Pre-increment
  iterator operator--(int);  // Post-decrement
  iterator& operator--();    // Pre-decrement

  bool operator==(const_iterator other) const;
  bool operator!=(const_iterator other) const;
};

// Освобождение памяти
template <typename T, typename Alloc>
void Vector<T, Alloc>::deallocateMemory_() {
  for (size_t i = 0; i < size_; i++)
    alloc_traits::destroy(allocator_, data_ + i);

  alloc_traits::deallocate(allocator_, data_, capacity_);
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::initInsert(size_t len) {
  for (size_t i = size_; i < len; ++i) {
    try {
      alloc_traits::construct(allocator_, data_ + i, T(T()));
    } catch (...) {
      for (size_t j = 0; j < i; ++j)
        alloc_traits::destroy(allocator_, data_ + j);

      alloc_traits::deallocate(allocator_, data_, size_);

      throw;
    }
  }
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::addBackValue(T* inputData, const T& value) {
  try {
    alloc_traits::construct(allocator_, inputData + size_, T(value));
  } catch (...) {
    for (size_t j = 0; j < size_; ++j)
      alloc_traits::destroy(allocator_, inputData + j);

    alloc_traits::deallocate(allocator_, inputData, size_);

    throw;
  }
}

// Копирование элементов
template <typename T, typename Alloc>
void Vector<T, Alloc>::copyElements_(const T* src) {
  data_ = alloc_traits::allocate(allocator_, capacity_);

  for (size_t i = 0; i < size_; ++i) {
    try {
      alloc_traits::construct(allocator_, data_ + i, T(src[i]));
    } catch (...) {
      for (size_t j = 0; j < i; ++j)
        alloc_traits::destroy(allocator_, data_ + j);

      alloc_traits::deallocate(allocator_, data_, size_);

      throw;
    }
  }
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::copyFrom(T* src) {
  for (size_t i = 0; i < size_; ++i) {
    try {
      alloc_traits::construct(allocator_, src + i, std::move(T(data_[i])));
    } catch (...) {
      for (size_t j = 0; j < i; ++j) alloc_traits::destroy(allocator_, src + j);

      alloc_traits::deallocate(allocator_, src, size_);
      throw;
    }
  }
}

template <typename T, typename Alloc>
inline Vector<T, Alloc>::Vector()
    : size_(0), capacity_(0), data_(nullptr), allocator_() {}

template <typename T, typename Alloc>
Vector<T, Alloc>::Vector(size_t size) : size_(size), capacity_(size) {
  if (size > 0) {
    data_ = alloc_traits::allocate(allocator_, size_);

    for (size_t i = 0; i < size_; ++i) {
      try {
        alloc_traits::construct(allocator_, data_ + i, T(T()));
      } catch (...) {
        for (size_t j = 0; j < i; j++)
          alloc_traits::destroy(allocator_, data_ + j);

        alloc_traits::deallocate(allocator_, data_, size_);

        throw;
      }
    }
  } else
    data_ = nullptr;
}

template <typename T, typename Alloc>
inline Vector<T, Alloc>::Vector(const std::initializer_list<T>& list)
    : Vector(list.size()) {
  std::copy(list.begin(), list.end(), data_);
}

template <typename T, typename Alloc>
inline Vector<T, Alloc>::Vector(const Vector& other)
    : size_(other.size_), capacity_(other.capacity_) {
  if (other.empty())
    data_ = nullptr;
  else
    copyElements_(other.data_);
}

template <typename T, typename Alloc>
inline Vector<T, Alloc>::Vector(Vector&& other) noexcept(
    alloc_traits::propagate_on_container_swap::value)
    : Vector() {
  if (this != &other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(allocator_, other.allocator_);
    std::swap(data_, other.data_);
  }
}

template <typename T, typename Alloc>
inline Vector<T, Alloc>::~Vector() {
  deallocateMemory_();

  data_ = nullptr;
  size_ = 0;
  capacity_ = 0;
}

template <typename T, typename Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator=(const Vector& other) noexcept {
  if (this != &other) {
    if (!empty()) deallocateMemory_();

    size_ = other.size_;
    capacity_ = other.capacity_;

    copyElements_(other.data_);
  }

  return *this;
}

template <typename T, typename Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator=(Vector&& other) noexcept {
  if (this != &other) {
    if (!empty()) deallocateMemory_();

    allocator_ = std::move(other.allocator_);
    data_ = std::move(other.data_);
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
  }

  return *this;
}

template <typename T, typename Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator=(
    std::initializer_list<T> const& list) {
  if (!empty() && list.size() > capacity_) {
    deallocateMemory_();
    data_ - alloc_traits::allocate(allocator_, list.size());
    capacity_ = list.size();
  } else if (!empty() && list.size() <= capacity_)
    clear();

  size_ = list.size();
  std::uninitialized_copy(list.begin(), list.end(), data_);

  return *this;
}

template <typename T, typename Alloc>
inline T& Vector<T, Alloc>::at(size_t index) {
  if (index >= size_) throw std::out_of_range("Index out of range");

  return data_[index];
}

template <typename T, typename Alloc>
inline const T& Vector<T, Alloc>::at(size_t index) const {
  if (index >= size_) throw std::out_of_range("Index out of range");

  return data_[index];
}

template <typename T, typename Alloc>
inline T& Vector<T, Alloc>::operator[](size_t index) {
  return data_[index];
}

template <typename T, typename Alloc>
inline const T& Vector<T, Alloc>::operator[](size_t index) const {
  return data_[index];
}

template <typename T, typename Alloc>
inline T& Vector<T, Alloc>::front() {
  return *data_;
}

template <typename T, typename Alloc>
inline const T& Vector<T, Alloc>::front() const {
  return *data_;
}

template <typename T, typename Alloc>
inline T& Vector<T, Alloc>::back() {
  return *(data_ + size_ - 1);
}

template <typename T, typename Alloc>
inline const T& Vector<T, Alloc>::back() const {
  return *(data_ + size_ - 1);
}

template <typename T, typename Alloc>
inline typename Vector<T, Alloc>::iterator Vector<T, Alloc>::data() noexcept {
  return data_;
}

template <typename T, typename Alloc>
inline typename Vector<T, Alloc>::const_iterator Vector<T, Alloc>::data()
    const noexcept {
  return data_;
}

template <typename T, typename Alloc>
inline typename Vector<T, Alloc>::iterator Vector<T, Alloc>::begin() noexcept {
  return data_;
}

template <typename T, typename Alloc>
inline typename Vector<T, Alloc>::iterator Vector<T, Alloc>::end() noexcept {
  return data_ + size_;
}

template <typename T, typename Alloc>
inline typename Vector<T, Alloc>::const_iterator Vector<T, Alloc>::begin()
    const noexcept {
  return data_;
}

template <typename T, typename Alloc>
inline typename Vector<T, Alloc>::const_iterator Vector<T, Alloc>::end()
    const noexcept {
  return data_ + size_;
}

template <typename T, typename Alloc>
inline size_t Vector<T, Alloc>::size() const noexcept {
  return size_;
}

template <typename T, typename Alloc>
inline size_t Vector<T, Alloc>::capacity() const noexcept {
  return capacity_;
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::reserve(size_t new_capacity) {
  if (new_capacity <= capacity_) return;

  T* new_data = alloc_traits::allocate(allocator_, new_capacity);
  for (size_t i = 0; i < size_; ++i)
    alloc_traits::construct(allocator_, new_data + i, std::move(data_[i]));

  deallocateMemory_();

  data_ = new_data;
  capacity_ = new_capacity;
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::shrink_to_fit() {
  if (size_ < capacity_) {
    T* new_data = alloc_traits::allocate(allocator_, size_);
    for (size_t i = 0; i < size_; ++i)
      alloc_traits::construct(allocator_, new_data + i, std::move(data_[i]));

    deallocateMemory_();

    data_ = new_data;
    capacity_ = size_;
  }
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::clear() {
  for (size_t i = 0; i < size_; ++i)
    alloc_traits::destroy(allocator_, data_ + i);

  size_ = 0;
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::push_back(const T& value) {
  if (size_ == capacity_) reserve(capacity_ == 0 ? 1 : capacity_ * 2);

  alloc_traits::construct(allocator_, data_ + size_, value);
  ++size_;
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::pop_back() {
  if (empty()) throw std::out_of_range("Vector is empty.");

  --size_;
  alloc_traits::destroy(allocator_, data_ + size_);
}

template <typename T, typename Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(const_iterator pos,
                                                             const T& value) {
  size_t index = pos - data_;
  push_back(value);
  for (size_t i = size_ - 1; i > index; --i) std::swap(data_[i], data_[i - 1]);

  return data_ + index;
}

template <typename T, typename Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(const_iterator pos,
                                                             T&& value) {
  size_t index = pos - data_;
  push_back(std::move(value));
  for (size_t i = size_ - 1; i > index; --i) std::swap(data_[i], data_[i - 1]);

  return data_ + index;
}

template <typename T, typename Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(const_iterator pos,
                                                             size_t count,
                                                             const T& value) {
  size_t index = pos - data_;
  for (size_t i = 0; i < count; ++i) push_back(value);
  for (size_t i = size_ - 1; i >= index + count; --i)
    std::swap(data_[i], data_[i - count]);

  return data_ + index;
}

template <typename T, typename Alloc>
T* Vector<T, Alloc>::erase(T* pos) {
  if (pos < begin() || pos >= end())
    throw std::out_of_range("Iterator out of range");

  size_t index = pos - begin();
  --size_;

  for (size_t i = index; i < index; ++i) data_[i] = std::move(data_[i + 1]);

  return begin() + index;
}

template <typename T, typename Alloc>
void Vector<T, Alloc>::swap(Vector& other) noexcept(
    alloc_traits::propagate_on_container_swap::value) {
  using std::swap;
  swap(data_, other.data_);
  swap(size_, other.size_);
  swap(capacity_, other.capacity_);
  swap(allocator_, other.allocator_);
}

template <typename T, typename Alloc>
inline bool Vector<T, Alloc>::empty() const noexcept {
  return size_ == 0;
}

template <typename T, typename Alloc>
template <typename... Args>
T* Vector<T, Alloc>::insert_many(const T* pos, Args&&... args) {
  if (pos < begin() || pos > end())
    throw std::out_of_range("Iterator out of range");

  size_t index = pos - begin();

  size_t count = (sizeof...(Args));

  if (size_ + count >= capacity_) reserve(size_ + count);

  initInsert(size_ + count);

  for (size_t i = size_ + count - 1; i >= index + count; --i)
    data_[i] = std::move(data_[i - count]);

  size_t i = index;
  ((data_[i++] = std::move(args)), ...);
  size_ += sizeof...(Args);

  return begin() + index;
}

template <typename T, typename Alloc>
template <typename... Args>
void Vector<T, Alloc>::insert_many_back(Args&&... args) {
  size_t count = (sizeof...(Args));

  if (size_ + count >= capacity_) reserve(size_ + count);

  initInsert(size_ + count);

  size_t index = size_;
  ((data_[index++] = std::move(args)), ...);
  size_ += sizeof...(Args);
}

template <typename T, typename Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::operator++(int) {
  iterator temp = *this;
  ++(*this);
  return temp;
}

template <typename T, typename Alloc>
typename Vector<T, Alloc>::iterator& Vector<T, Alloc>::operator++() {
  ++data_;
  return *this;
}

template <typename T, typename Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::operator--(int) {
  iterator temp = *this;
  --(*this);
  return temp;
}

template <typename T, typename Alloc>
typename Vector<T, Alloc>::iterator& Vector<T, Alloc>::operator--() {
  --data_;
  return *this;
}

template <typename T, typename Alloc>
bool Vector<T, Alloc>::operator==(const_iterator other) const {
  return data_ == other;
}

template <typename T, typename Alloc>
bool Vector<T, Alloc>::operator!=(const_iterator other) const {
  return data_ != other;
}

}  // namespace s21

#endif  // SRC_S21_VECTOR__H
