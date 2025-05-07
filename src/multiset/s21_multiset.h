#ifndef SRC_MULTISET_H
#define SRC_MULTISET_H

#include "../map/avl_tree.h"  // Подключаем AVLTree

namespace s21 {

template <typename Key>
class Multiset {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = typename AVLTree<Key, Key>::iterator;
  using const_iterator = typename AVLTree<Key, Key>::const_iterator;
  using size_type = size_t;

  // Конструкторы
  Multiset() = default;

  Multiset(std::initializer_list<key_type> const& items) {
    for (const auto& item : items) {
      insert(item);
    }
  }

  Multiset(const Multiset& other) : tree_(other.tree_) {}

  Multiset(Multiset&& other) noexcept : tree_(std::move(other.tree_)) {}

  ~Multiset() = default;

  // Операторы присваивания
  Multiset& operator=(const Multiset& other) {
    if (this != &other) {
      tree_ = other.tree_;
    }
    return *this;
  }

  Multiset& operator=(Multiset&& other) noexcept {
    if (this != &other) {
      tree_ = std::move(other.tree_);
    }
    return *this;
  }

  // Итераторы
  iterator begin() noexcept { return tree_.begin(); }
  const_iterator begin() const noexcept { return tree_.begin(); }

  iterator end() noexcept { return tree_.end(); }
  const_iterator end() const noexcept { return tree_.end(); }

  // Вставка элемента (дубликаты разрешены)
  iterator insert(const value_type& value) {
    return tree_.insert(value, value, true).first;
  }

  // Удаление элемента
  void erase(iterator pos) { tree_.erase(pos); }

  // Очистка множества
  void clear() noexcept { tree_.clear(); }

  // Поиск элемента
  iterator find(const key_type& key) { return tree_.find(key); }
  const_iterator find(const key_type& key) const { return tree_.find(key); }

  // Проверка наличия элемента
  bool contains(const key_type& key) const { return tree_.contains(key); }

  // Количество элементов
  size_type size() const noexcept { return tree_.size(); }

  // Проверка на пустоту
  bool empty() const noexcept { return tree_.empty(); }

  // Обмен содержимым
  void swap(Multiset& other) { tree_.swap(other.tree_); }

  // Количество элементов с определенным ключом
  size_type count(const key_type& key) const {
    auto range = equal_range(key);
    return std::distance(range.first, range.second);
  }

  // Возвращает диапазон элементов с определенным ключом
  std::pair<iterator, iterator> equal_range(const key_type& key) {
    return {tree_.lower_bound(key), tree_.upper_bound(key)};
  }

  std::pair<const_iterator, const_iterator> equal_range(
      const key_type& key) const {
    return {tree_.lower_bound(key), tree_.upper_bound(key)};
  }

  // Возвращает итератор на первый элемент, не меньший ключа
  iterator lower_bound(const key_type& key) { return tree_.lower_bound(key); }
  const_iterator lower_bound(const key_type& key) const {
    return tree_.lower_bound(key);
  }

  // Возвращает итератор на первый элемент, больший ключа
  iterator upper_bound(const key_type& key) { return tree_.upper_bound(key); }
  const_iterator upper_bound(const key_type& key) const {
    return tree_.upper_bound(key);
  }

 private:
  AVLTree<Key, Key> tree_;  // Используем AVLTree для хранения данных
};

}  // namespace s21

#endif  // SRC_MULTISET_H
