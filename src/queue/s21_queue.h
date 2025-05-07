#ifndef SRC_S21_QUEUE_H
#define SRC_S21_QUEUE_H

#include <iostream>
#include <memory>

namespace s21 {

/**
 * @brief Класс, реализующий очередь с использованием массива.
 *
 * @tparam T Тип элементов, хранящихся в очереди.
 */
template <typename T>
class Queue {
 private:
  std::unique_ptr<T[]>
      data_;  ///< Умный указатель на массив, хранящий элементы очереди
  size_t size_;      ///< Текущий размер очереди
  size_t capacity_;  ///< Вместимость массива
  size_t front_;  ///< Индекс первого элемента в очереди
  size_t back_;  ///< Индекс следующего элемента для добавления

  void resize(size_t new_capacity);  ///< Метод для изменения размера массива

 public:
  /**
   * @brief Конструктор по умолчанию, создающий пустую очередь.
   */
  Queue();

  /**
   * @brief Копирующий конструктор.
   * @param other Очередь, из которой будет скопировано содержимое.
   */
  Queue(const Queue &other);

  /**
   * @brief Перемещающий конструктор.
   * @param other Очередь, из которой будет перемещено содержимое.
   */
  Queue(Queue &&other) noexcept;

  /**
   * @brief Деструктор.
   */
  ~Queue() = default;

  /**
   * @brief Оператор присваивания для копирования.
   * @param other Очередь, из которой будет скопировано содержимое.
   * @return Ссылка на текущий объект.
   */
  Queue &operator=(const Queue &other);

  /**
   * @brief Оператор присваивания для перемещения.
   * @param other Очередь, из которой будет перемещено содержимое.
   * @return Ссылка на текущий объект.
   */
  Queue &operator=(Queue &&other) noexcept;

  /**
   * @brief Проверка, пуста ли очередь.
   * @return true, если очередь пуста; false в противном случае.
   */
  bool empty() const noexcept;

  /**
   * @brief Возвращает количество элементов в очереди.
   * @return Количество элементов.
   */
  size_t size() const noexcept;

  /**
   * @brief Добавляет элемент в очередь.
   * @param value Элемент, который будет добавлен.
   */
  void push(const T &value);

  /**
   * @brief Удаляет элемент из очереди.
   */
  void pop();

  /**
   * @brief Возвращает первый элемент очереди.
   * @return Константная ссылка на первый элемент.
   */
  const T &front() const;

  /**
   * @brief Возвращает последний элемент очереди.
   * @return Константная ссылка на последний элемент.
   */
  const T &back() const;

  /**
   * @brief Обмен значениями с другой очередью.
   * @param other Очередь, с которой будет произведен обмен.
   */
  void swap(Queue &other);

  /**
   * @brief Очищает очередь, удаляя все элементы.
   */
  void clear() noexcept;

  /**
   * @brief Итератор для доступа к элементам очереди.
   */

  template <typename... Args>
  void insert_many_back(Args &&...args) {
    (push(std::forward<Args>(args)), ...);
  }

  class Iterator {
   private:
    T *ptr_;  ///< Указатель на текущий элемент

   public:
    // Конструктор итератора
    Iterator(T *ptr) : ptr_(ptr) {}

    // Оператор разыменования
    T &operator*() { return *ptr_; }

    // Оператор инкремента
    Iterator &operator++() {
      ++ptr_;
      return *this;
    }

    // Оператор сравнения
    bool operator!=(const Iterator &other) const { return ptr_ != other.ptr_; }

    // Оператор сравнения для равенства
    bool operator==(const Iterator &other) const { return ptr_ == other.ptr_; }
  };

  // Методы для получения итераторов
  Iterator begin() { return Iterator(data_.get()); }
  Iterator end() { return Iterator(data_.get() + size_); }
};

template <typename T>
Queue<T>::Queue() : size_(0), capacity_(1), front_(0), back_(0) {
  data_ = std::make_unique<T[]>(capacity_);
}

template <typename T>
Queue<T>::Queue(const Queue &other)
    : size_(other.size_),
      capacity_(other.capacity_),
      front_(other.front_),
      back_(other.back_) {
  data_ = std::make_unique<T[]>(capacity_);
  for (size_t i = 0; i < size_; ++i)
    data_[i] = other.data_[(front_ + i) % capacity_];
}

template <typename T>
Queue<T>::Queue(Queue &&other) noexcept
    : data_(std::move(other.data_)),
      size_(other.size_),
      capacity_(other.capacity_),
      front_(other.front_),
      back_(other.back_) {
  other.size_ = 0;
  other.capacity_ = 0;
  other.front_ = 0;
  other.back_ = 0;
}

template <typename T>
Queue<T> &Queue<T>::operator=(const Queue &other) {
  if (this != &other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    front_ = other.front_;
    back_ = other.back_;
    data_ = std::make_unique<T[]>(capacity_);
    for (size_t i = 0; i < size_; ++i) {
      data_[i] = other.data_[(front_ + i) % capacity_];
    }
  }

  return *this;
}

template <typename T>
Queue<T> &Queue<T>::operator=(Queue &&other) noexcept {
  if (this != &other) {
    data_ = std::move(other.data_);
    size_ = other.size_;
    capacity_ = other.capacity_;
    front_ = other.front_;
    back_ = other.back_;
    other.size_ = 0;
    other.capacity_ = 0;
    other.front_ = 0;
    other.back_ = 0;
  }

  return *this;
}

template <typename T>
bool Queue<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T>
size_t Queue<T>::size() const noexcept {
  return size_;
}

template <typename T>
void Queue<T>::push(const T &value) {
  if (size_ == capacity_) resize(capacity_ * 2);

  data_[back_] = value;
  back_ = (back_ + 1) % capacity_;
  size_++;
}

template <typename T>
void Queue<T>::pop() {
  if (empty()) throw std::out_of_range("Queue is empty");
  front_ = (front_ + 1) % capacity_;
  size_--;
}

template <typename T>
const T &Queue<T>::front() const {
  if (empty()) throw std::out_of_range("Queue is empty");
  return data_[front_];
}

template <typename T>
const T &Queue<T>::back() const {
  if (empty()) throw std::out_of_range("Queue is empty");
  return data_[(back_ - 1 + capacity_) % capacity_];
}

template <typename T>
void Queue<T>::swap(Queue &other) {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
  std::swap(front_, other.front_);
  std::swap(back_, other.back_);
}

template <typename T>
void Queue<T>::resize(size_t new_capacity) {
  std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_capacity);
  for (size_t i = 0; i < size_; ++i) {
    new_data[i] = data_[(front_ + i) % capacity_];
  }
  data_ = std::move(new_data);
  capacity_ = new_capacity;
  front_ = 0;
  back_ = size_;
}

template <typename T>
void Queue<T>::clear() noexcept {
  size_ = 0;
  front_ = 0;
  back_ = 0;
}

}  // namespace s21

#endif  // SRC_S21_QUEUE_H
