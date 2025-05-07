#ifndef SRC_SET__H
#define SRC_SET__H

#include "../map/avl_tree.h"

namespace s21 {

template <typename Key>
class Set {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = typename AVLTree<Key, Key>::iterator;
  using const_iterator = typename AVLTree<Key, Key>::const_iterator;
  using size_type = size_t;

  Set() : tree_() {}

  Set(std::initializer_list<key_type> const& items) {
    for (const auto& item : items) {
      insert(item);
    }
  }

  Set(const Set& other) : tree_(other.tree_) {}

  Set(Set&& other) noexcept : tree_(std::move(other.tree_)) {}

  ~Set() = default;

  Set& operator=(const Set& other) {
    if (this != &other) {
      tree_ = other.tree_;
    }
    return *this;
  }

  Set& operator=(Set&& other) noexcept {
    if (this != &other) {
      tree_ = std::move(other.tree_);
    }
    return *this;
  }

  iterator begin() noexcept { return tree_.begin(); }
  const_iterator begin() const noexcept { return tree_.begin(); }

  iterator end() noexcept { return tree_.end(); }
  const_iterator end() const noexcept { return tree_.end(); }

  bool empty() const noexcept { return tree_.empty(); }

  size_type size() const noexcept { return tree_.size(); }

  size_type max_size() const noexcept { return tree_.max_size(); }

  void clear() noexcept { tree_.clear(); }

  std::pair<iterator, bool> insert(const key_type& key) {
    return tree_.insert(key, key);
  }

  void erase(iterator pos) { tree_.erase(pos); }

  void swap(Set& other) { tree_.swap(other.tree_); }

  void merge(Set& other) {
    for (auto it = other.begin(); it != other.end(); ++it) {
      auto res = insert(*it);
      if (res.second) other.erase(res.first);
    }
  }

  bool contains(const key_type& key) const { return tree_.contains(key); }

  iterator find(const key_type& key) { return tree_.find(key); }
  const_iterator find(const key_type& key) const { return tree_.find(key); }

 private:
  AVLTree<Key, Key> tree_;
};

}  // namespace s21

#endif  // SRC_SET__H
