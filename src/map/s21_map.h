#ifndef S21_MAP_H_
#define S21_MAP_H_

#include <vector>

#include "avl_tree.h"

namespace s21 {
template <typename Key, typename T>
class map : public AVLTree<Key, T> {
 public:
  class MapIterator;
  class ConstMapIterator;

  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = MapIterator;
  using const_iterator = ConstMapIterator;
  using size_type = size_t;

  // MapMemberFunctions
  map() : AVLTree<Key, T>() {};

  map(const std::initializer_list<value_type> &items) {
    for (auto i = items.begin(); i != items.end(); ++i) insert(*i);
  }

  map(const map &m) : AVLTree<Key, T>(m) {};

  map(map &&m) noexcept : AVLTree<Key, T>(std::move(m)) {};

  map &operator=(map &&m) noexcept {
    if (this != &m) AVLTree<Key, T>::operator=(std::move(m));

    return *this;
  }

  map &operator=(const map &m) {
    if (this != &m) AVLTree<Key, T>::operator=(m);

    return *this;
  }

  ~map() = default;

  T &at(const Key &key) {
    auto iter = find(key);

    if (iter == this->end()) {
      throw std::out_of_range(
          "Container does not have an element with the specified key");
    }

    return iter.return_value();
  }

  T &operator[](const Key &key) {
    auto iter = find(key);

    if (iter == end()) {
      auto res = insert(std::make_pair(key, T()));
      iter = res.first;
    }

    return iter.return_value();
  }

  // MapIterators
  iterator begin() {
    return map<Key, T>::MapIterator(
        AVLTree<Key, T>::GetMinNode(AVLTree<Key, T>::root));
  }

  iterator end() {
    if (AVLTree<Key, T>::root == nullptr) return begin();
    typename AVLTree<Key, T>::node *max_node =
        AVLTree<Key, T>::GetMaxNode(AVLTree<Key, T>::root);
    MapIterator ptr(nullptr, max_node);
    return ptr;
  }

  const_iterator constBegin() const {
    return map<Key, T>::ConstMapIterator(
        AVLTree<Key, T>::GetMinNode(AVLTree<Key, T>::root));
  }

  const_iterator constEnd() const {
    if (AVLTree<Key, T>::root == nullptr) return constBegin();

    typename AVLTree<Key, T>::node *max_node =
        AVLTree<Key, T>::GetMaxNode(AVLTree<Key, T>::root);
    ConstMapIterator ptr(nullptr, max_node);

    return ptr;
  }

  // MapCapacity
  bool empty() { return AVLTree<Key, T>::root == nullptr; }

  size_type size() { return AVLTree<Key, T>::size(); }

  size_type max_size() { return AVLTree<Key, T>::size_max(); }

  // MapModifiers
  std::pair<iterator, bool> insert(const value_type &value) {
    return insert(value.first, value.second);
  }

  std::pair<iterator, bool> insert(const Key &key, const T &obj) {
    std::pair<iterator, bool> res;

    if (AVLTree<Key, T>::root == nullptr) {
      AVLTree<Key, T>::root = new typename AVLTree<Key, T>::node(key, obj);
      res.first = iterator(AVLTree<Key, T>::root);
      res.second = true;
    } else {
      bool status =
          AVLTree<Key, T>::RecursiveInsertion(AVLTree<Key, T>::root, key, obj);
      res.first = find(key);
      res.second = status;
    }

    return res;
  }

  std::pair<iterator, bool> insert_or_assign(const Key &key, const T &obj) {
    auto ptr = find(key);

    if (ptr != this->end()) {
      erase(ptr);
      auto state = insert(key, obj);
      state.second = false;
      return state;
    }

    return insert(key, obj);
  }

  void erase(iterator pos) {
    if (AVLTree<Key, T>::root == nullptr || pos.it_node == nullptr) return;
    AVLTree<Key, T>::root =
        AVLTree<Key, T>::RecursiveDeleting(AVLTree<Key, T>::root, (*pos).first);
  }

  void swap(map &other) { AVLTree<Key, T>::swap(other); }

  void merge(map &other) {
    map c_tree(other);
    iterator c_it = c_tree.begin();
    for (; c_it != c_tree.end(); ++c_it) {
      auto key = (*c_it).first;
      auto obj = (*c_it).second;
      std::pair<iterator, bool> res = insert(key, obj);
      if (res.second) other.erase(res.first);
    }
  }

  // MapLookup
  bool contains(const key_type &key) { return AVLTree<Key, T>::contains(key); }

  // ClassMapIterators
  class MapIterator : public AVLTree<Key, T>::Iterator {
   public:
    friend class map;
    MapIterator() : AVLTree<Key, T>::Iterator() {};
    explicit MapIterator(typename AVLTree<Key, T>::node *Node,
                         typename AVLTree<Key, T>::node *pastNode = nullptr)
        : AVLTree<Key, T>::Iterator(Node, pastNode) {};
    value_type operator*() {
      if (AVLTree<Key, T>::Iterator::it_node == nullptr) return {};

      return std::make_pair(AVLTree<Key, T>::Iterator::it_node->key_,
                            AVLTree<Key, T>::Iterator::it_node->value_);
    }

   protected:
    T &return_value() {
      if (AVLTree<Key, T>::Iterator::it_node == nullptr) {
        static T imagine_val{};
        return imagine_val;
      }

      return AVLTree<Key, T>::Iterator::it_node->value_;
    }
  };

  class ConstMapIterator : public MapIterator {
   public:
    friend class map;
    ConstMapIterator() : MapIterator() {};
    explicit ConstMapIterator(
        typename AVLTree<Key, T>::node *Node,
        typename AVLTree<Key, T>::node *pastNode = nullptr)
        : MapIterator(Node, pastNode) {};
    const_iterator operator*() const { return MapIterator::operator*(); };
  };

  template <class... Args>
  std::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    std::vector<std::pair<typename map<Key, T>::iterator, bool>>
        inserted_arguments;

    for (const auto &arg : {args...}) inserted_arguments.push_back(insert(arg));

    return inserted_arguments;
  }

 private:
  iterator find(const Key &key) {
    typename AVLTree<Key, T>::node *searched_node =
        AVLTree<Key, T>::RecursiveSearch(AVLTree<Key, T>::root, key);
    return iterator(searched_node);
  }
};

}  // namespace s21

#endif  // S21_MAP_H_
