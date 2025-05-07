#ifndef SRC_S21_STACK__H
#define SRC_S21_STACK__H

#include <initializer_list>
#include <iostream>
#include <memory>
#include <stdexcept>

#define MIN_STACK_SIZE 256

namespace s21 {

/**
 * @brief Класс Стек, реализованный с помощью динамического массива.
 *
 * @tparam T Тип элементов, хранящихся в стеке.
 */
template <typename T>
class Stack {
 private:
  std::unique_ptr<T[]>
      data_;  ///< Умный указатель на массив, хранящий элементы стека.
  size_t size_;      ///< Текущий размер стека.
  size_t capacity_;  ///< Вместимость стека.

  void resize(size_t new_capacity);  ///< Увеличивает размер массива.
  void destroy_elements();  ///< Уничтожает элементы стека.

 public:
  /**
   * @brief Конструктор по умолчанию.
   */
  Stack();

  /**
   * @brief Конструктор копирования.
   * @param other Другой стек, который будет скопирован.
   */
  Stack(const Stack& other);

  /**
   * @brief Конструктор инициализации с помощью списка инициализации.
   * @param src Список инициализации.
   */
  Stack(std::initializer_list<T> const src);

  /**
   * @brief Конструктор перемещения.
   * @param other Другой стек, который будет перемещен.
   */
  Stack(Stack&& other) noexcept;

  /**
   * @brief Деструктор.
   */
  ~Stack() = default;

  /**
   * @brief Оператор присваивания копированием.
   * @param other Другой стек для копирования.
   * @return Ссылка на текущий объект.
   */
  Stack& operator=(const Stack& other);

  /**
   * @brief Оператор присваивания перемещением.
   * @param other Другой стек, который будет перемещен.
   * @return Ссылка на текущий объект.
   */
  Stack& operator=(Stack&& other) noexcept;

  /**
   * @brief Проверяет, пуст ли стек.
   * @return true, если стек пуст; иначе false.
   */
  bool empty() const noexcept;

  /**
   * @brief Возвращает текущее количество элементов в стеке.
   * @return Текущий размер стека.
   */
  size_t size() const noexcept;

  /**
   * @brief Добавляет элемент на вершину стека.
   * @param value Элемент, который будет добавлен.
   */
  void push(T value);

  /**
   * @brief Удаляет верхний элемент из стека.
   * @throw std::out_of_range Если стек пуст.
   */
  void pop();

  /**
   * @brief Возвращает верхний элемент стека.
   * @return Ссылку на верхний элемент.
   * @throw std::out_of_range Если стек пуст.
   */
  T& top();

  /**
   * @brief Возвращает верхний элемент стека (константная версия).
   * @return Константная ссылка на верхний элемент.
   * @throw std::out_of_range Если стек пуст.
   */
  const T& top() const;

  /**
   * @brief Меняет содержимое двух стеков.
   * @param other Другой стек для обмена содержимым.
   */
  void swap(Stack& other);

  /**
   * @brief Очищает стек, удаляя все элементы.
   */
  void clear() noexcept;

  template <typename... Args>
  void insert_many_back(Args&&... args);

  /**
   * @brief Итератор для стека.
   */
  class iterator {
   private:
    T* ptr_;  ///< Указатель на текущий элемент.

   public:
    using difference_type = std::ptrdiff_t;  ///< Тип разности итераторов.

    /**
     * @brief Конструктор итератора.
     * @param ptr Указатель на текущий элемент.
     */
    iterator(T* ptr) : ptr_(ptr) {}

    /**
     * @brief Разыменование итератора.
     * @return Ссылку на текущее значение.
     */
    T& operator*() { return *ptr_; }

    /**
     * @brief Получение указателя на текущее значение.
     * @return Указатель на текущее значение.
     */
    T* operator->() { return ptr_; }

    /**
     * @brief Префиксный инкремент итератора.
     * @return Ссылку на текущий итератор.
     */
    iterator& operator++() {
      ++ptr_;
      return *this;
    }

    /**
     * @brief Постфиксный инкремент итератора.
     * @return Копию текущего итератора до инкремента.
     */
    iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    /**
     * @brief Префиксный декремент итератора.
     * @return Ссылку на текущий итератор.
     */
    iterator& operator--() {
      --ptr_;
      return *this;
    }

    /**
     * @brief Постфиксный декремент итератора.
     * @return Копию текущего итератора до декремента.
     */
    iterator operator--(int) {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }

    /**
     * @brief Сложение итератора с целым числом.
     * @param n Смещение.
     * @return Новый итератор, сдвинутый на n.
     */
    iterator operator+(difference_type n) const { return iterator(ptr_ + n); }

    /**
     * @brief Вычитание целого числа из итератора.
     * @param n Смещение.
     * @return Новый итератор, сдвинутый на -n.
     */
    iterator operator-(difference_type n) const { return iterator(ptr_ - n); }

    /**
     * @brief Вычисляет разность между двумя итераторами.
     * @param other Другой итератор.
     * @return Разность между текущим и другим итератором.
     */
    difference_type operator-(const iterator& other) const {
      return ptr_ - other.ptr_;
    }

    /**
     * @brief Сравнение итераторов на равенство.
     * @param other Другой итератор для сравнения.
     * @return true, если итераторы равны; иначе false.
     */
    bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }

    /**
     * @brief Сравнение итераторов на неравенство.
     * @param other Другой итератор для сравнения.
     * @return true, если итераторы не равны; иначе false.
     */
    bool operator!=(const iterator& other) const { return !(*this == other); }

    /**
     * @brief Сравнение итераторов по меньше.
     * @param other Другой итератор для сравнения.
     * @return true, если текущий итератор меньше; иначе false.
     */
    bool operator<(const iterator& other) const { return ptr_ < other.ptr_; }

    /**
     * @brief Сравнение итераторов по меньше или равно.
     * @param other Другой итератор для сравнения.
     * @return true, если текущий итератор меньше или равен; иначе false.
     */
    bool operator<=(const iterator& other) const { return ptr_ <= other.ptr_; }

    /**
     * @brief Сравнение итераторов по больше.
     * @param other Другой итератор для сравнения.
     * @return true, если текущий итератор больше; иначе false.
     */
    bool operator>(const iterator& other) const { return ptr_ > other.ptr_; }

    /**
     * @brief Сравнение итераторов по больше или равно.
     * @param other Другой итератор для сравнения.
     * @return true, если текущий итератор больше или равен; иначе false.
     */
    bool operator>=(const iterator& other) const { return ptr_ >= other.ptr_; }
  };

  iterator begin() {
    return iterator(data_.get());
  }  ///< Возвращает итератор на начало стека.
  iterator end() {
    return iterator(data_.get() + size_);
  }  ///< Возвращает итератор на конец стека.
};

template <typename T>
inline Stack<T>::Stack() : size_(0), capacity_(MIN_STACK_SIZE) {
  data_ = std::make_unique<T[]>(capacity_);
}

template <typename T>
inline Stack<T>::Stack(const Stack& other)
    : size_(other.size_), capacity_(other.capacity_) {
  data_ = std::make_unique<T[]>(capacity_);
  for (size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
}

template <typename T>
Stack<T>::Stack(std::initializer_list<T> src)
    : size_(src.size()), capacity_(src.size()) {
  data_ = std::make_unique<T[]>(capacity_);
  size_t index = 0;
  for (const auto& item : src) data_[index++] = item;
}

template <typename T>
inline Stack<T>::Stack(Stack&& other) noexcept
    : data_(std::move(other.data_)),
      size_(other.size_),
      capacity_(other.capacity_) {
  other.size_ = 0;
  other.capacity_ = 0;
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
  if (this != &other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = std::make_unique<T[]>(capacity_);
    for (size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
  }

  return *this;
}

template <typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept {
  if (this != &other) {
    data_ = std::move(other.data_);
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  return *this;
}

template <typename T>
inline bool Stack<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T>
inline size_t Stack<T>::size() const noexcept {
  return size_;
}

template <typename T>
inline void Stack<T>::push(T value) {
  if (size_ == capacity_) resize(capacity_ * 2);
  data_[size_++] = value;
}

template <typename T>
inline void Stack<T>::pop() {
  if (empty()) throw std::out_of_range("Stack is empty");
  --size_;
}

template <typename T>
inline T& Stack<T>::top() {
  if (empty()) throw std::out_of_range("Stack is empty");
  return data_[size_ - 1];
}

template <typename T>
inline const T& Stack<T>::top() const {
  if (empty()) throw std::out_of_range("Stack is empty");
  return data_[size_ - 1];
}

template <typename T>
void Stack<T>::swap(Stack& other) {
  std::swap(data_, other.data_);
  std::swap(size_, other.size_);
  std::swap(capacity_, other.capacity_);
}

template <typename T>
void Stack<T>::clear() noexcept {
  size_ = 0;
}

template <typename T>
void Stack<T>::resize(size_t new_capacity) {
  std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_capacity);

  for (size_t i = 0; i < size_; ++i) new_data[i] = std::move(data_[i]);

  data_ = std::move(new_data);
  capacity_ = new_capacity;
}

template <typename T>
template <typename... Args>
void Stack<T>::insert_many_back(Args&&... args) {
  (push(std::forward<Args>(args)), ...);
}

}  // namespace s21

#endif  // SRC_S21_STACK__H
