#ifndef SRC_AVL_TREE_H
#define SRC_AVL_TREE_H

#include <iostream>
#include <limits>
#include <memory>
#include <utility>

namespace s21 {

template <typename Key, typename Value>
class AVLTree {
 protected:
  struct node;

 public:
  class Iterator;
  class ConstIterator;

  using key_type = Key;
  using value_type = Value;
  using reference = value_type&;
  using const_reference = const value_type&;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  using size_type = size_t;

  class Iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using pointer = Key*;
    using reference = Key&;

    Iterator() : it_node(nullptr), it_prev_node(nullptr) {}
    explicit Iterator(node* Node, node* node_past = nullptr)
        : it_node(Node), it_prev_node(node_past) {}

    iterator& operator++() {
      if (it_node == nullptr) return *this;

      if (it_node->right_ != nullptr) {
        it_node = GetMinNode(it_node->right_);
      } else {
        node* prev = it_node;
        it_node = it_node->parent_;
        while (it_node != nullptr && prev == it_node->right_) {
          prev = it_node;
          it_node = it_node->parent_;
        }
      }
      return *this;
    }

    iterator operator++(int) {
      Iterator tmp = *this;
      operator++();
      return tmp;
    }

    iterator& operator--() {
      if (it_node == nullptr) return *this;

      if (it_node->left_ != nullptr) {
        it_node = GetMaxNode(it_node->left_);
      } else {
        node* prev = it_node;
        it_node = it_node->parent_;
        while (it_node != nullptr && prev == it_node->left_) {
          prev = it_node;
          it_node = it_node->parent_;
        }
      }
      return *this;
    }

    iterator operator--(int) {
      Iterator tmp = *this;
      operator--();
      return tmp;
    }

    reference operator*() {
      if (it_node == nullptr) {
        throw std::out_of_range("Trying to dereference end() iterator");
      }
      return it_node->key_;
    }

    bool operator==(const Iterator& other) const noexcept {
      return it_node == other.it_node;
    }

    bool operator!=(const Iterator& other) const noexcept {
      return it_node != other.it_node;
    }

    friend class AVLTree<Key, Value>;

    node* get_node() const { return it_node; }

   protected:
    node* it_node;
    node* it_prev_node;

    node* MoveForward(node* Node) {
      if (Node->right_ != nullptr) return GetMinNode(Node->right_);
      auto parent = Node->parent_;
      while (parent != nullptr && Node == parent->right_) {
        Node = parent;
        parent = Node->parent_;
      }
      return parent;
    }

    node* MoveBack(node* Node) {
      if (Node->left_ != nullptr) return GetMaxNode(Node->left_);
      return nullptr;
    }
  };

  class ConstIterator : public Iterator {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = const Key;
    using difference_type = std::ptrdiff_t;
    using pointer = const Key*;
    using reference = const Key&;

    ConstIterator() : Iterator() {}

    // Конструктор, принимающий node*
    explicit ConstIterator(node* Node, node* node_past = nullptr)
        : Iterator(Node, node_past) {}

    const_reference operator*() const { return Iterator::operator*(); }
  };

  AVLTree() : root(nullptr) {}

  AVLTree(const AVLTree& other) { root = CopyTree(other.root, nullptr); }

  AVLTree(AVLTree&& other) noexcept : root(other.root) { other.root = nullptr; }

  ~AVLTree() { clear(); }

  AVLTree& operator=(AVLTree&& other) noexcept {
    if (this != &other) {
      root = other.root;
      other.root = nullptr;
    }
    return *this;
  }

  AVLTree& operator=(const AVLTree& other) {
    if (this != &other) {
      AVLTree temp(other);
      clear();
      *this = std::move(temp);
    }
    return *this;
  }

  iterator begin() noexcept { return iterator(GetMinNode(root)); }
  const_iterator begin() const noexcept {
    return const_iterator(GetMinNode(root));
  }

  iterator end() noexcept { return iterator(nullptr, GetMaxNode(root)); }
  const_iterator end() const noexcept {
    return const_iterator(nullptr, GetMaxNode(root));
  }

  bool empty() const noexcept { return root == nullptr; }

  size_t size() const { return RecursiveSize(root); }

  size_type max_size() const noexcept {
    return (std::numeric_limits<size_type>::max() / 2 - sizeof(Key) -
            sizeof(node)) /
           sizeof(node);
  }

  void clear() {
    if (root != nullptr) FreeNode(root);
    root = nullptr;
  }

  std::pair<iterator, bool> insert(const key_type& key,
                                   const value_type& value = value_type(),
                                   bool allow_duplicates = false) {
    if (root == nullptr) {
      root = new node(key, value);
      return {iterator(root), true};
    } else {
      bool status_insert =
          RecursiveInsertion(root, key, value, allow_duplicates);
      return {find(key), status_insert};
    }
  }

  void erase(iterator pos) {
    if (root == nullptr || pos.it_node == nullptr) return;
    root = RecursiveDeleting(root, *pos);
  }

  void swap(AVLTree& other) { std::swap(root, other.root); }

  void merge(AVLTree& other) {
    AVLTree tree(other);
    for (auto it = tree.begin(); it != tree.end(); ++it) {
      auto res = insert(*it);
      if (res.second) other.erase(res.first);
    }
  }

  bool contains(const key_type& key) const {
    return RecursiveSearch(root, key) != nullptr;
  }

  iterator find(const key_type& key) {
    return iterator(RecursiveSearch(root, key));
  }
  const_iterator find(const key_type& key) const {
    return const_iterator(RecursiveSearch(root, key));
  }

  iterator lower_bound(const key_type& key) {
    node* current = root;
    node* result = nullptr;
    while (current != nullptr) {
      if (!(current->key_ < key)) {
        result = current;
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return iterator(result);
  }

  const_iterator lower_bound(const key_type& key) const {
    node* current = root;
    node* result = nullptr;
    while (current != nullptr) {
      if (!(current->key_ < key)) {
        result = current;
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return const_iterator(result);
  }

  iterator upper_bound(const key_type& key) {
    node* current = root;
    node* result = nullptr;
    while (current != nullptr) {
      if (key < current->key_) {
        result = current;
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return iterator(result);
  }

  const_iterator upper_bound(const key_type& key) const {
    node* current = root;
    node* result = nullptr;
    while (current != nullptr) {
      if (key < current->key_) {
        result = current;
        current = current->left_;
      } else {
        current = current->right_;
      }
    }
    return const_iterator(result);
  }

 protected:
  struct node {
    node(key_type key, value_type value, node* parent = nullptr)
        : key_(key),
          value_(value),
          parent_(parent),
          left_(nullptr),
          right_(nullptr),
          height_(0) {}

    key_type key_;
    value_type value_;
    node* parent_;
    node* left_;
    node* right_;
    int height_;

    friend class AVLTree<Key, Value>;
  };

  node* root;

  void FreeNode(node* Node) {
    if (Node == nullptr) return;
    FreeNode(Node->left_);
    FreeNode(Node->right_);
    delete Node;
  }

  node* CopyTree(node* Node, node* parent) {
    if (Node == nullptr) return nullptr;
    node* new_node = new node(Node->key_, Node->value_, parent);
    new_node->left_ = CopyTree(Node->left_, new_node);
    new_node->right_ = CopyTree(Node->right_, new_node);
    return new_node;
  }

  static void SwapValue(node* x, node* y) {
    std::swap(x->key_, y->key_);
    std::swap(x->value_, y->value_);
  }

  void RightRotation(node* Node) {
    node* new_left = Node->left_->left_;
    node* new_right = Node->right_;
    node* new_left_right = Node->left_->right_;
    SwapValue(Node, Node->left_);
    Node->right_ = Node->left_;

    Node->left_ = new_left;
    if (Node->left_) {
      Node->left_->parent_ = Node;
    }

    Node->right_->left_ = new_left_right;
    if (Node->right_->left_) {
      Node->right_->left_->parent_ = Node->right_;
    }

    Node->right_->right_ = new_right;
    if (Node->right_->right_) {
      Node->right_->right_->parent_ = Node->right_;
    }

    SetHeight(Node->right_);
    SetHeight(Node);
  }

  void LeftRotation(node* Node) {
    node* new_left = Node->left_;
    node* new_right = Node->right_->right_;
    node* new_right_left = Node->right_->left_;
    SwapValue(Node, Node->right_);
    Node->left_ = Node->right_;

    Node->right_ = new_right;
    if (Node->right_) {
      Node->right_->parent_ = Node;
    }

    Node->left_->right_ = new_right_left;
    if (Node->left_->right_) {
      Node->left_->right_->parent_ = Node->left_;
    }

    Node->left_->left_ = new_left;
    if (Node->left_->left_) {
      Node->left_->left_->parent_ = Node->left_;
    }

    SetHeight(Node->left_);
    SetHeight(Node);
  }

  void Balancing(node* Node) {
    int balance = GetBalanceNum(Node);
    if (balance == -2) {
      if (GetBalanceNum(Node->left_) == 1) LeftRotation(Node->left_);
      RightRotation(Node);
    } else if (balance == 2) {
      if (GetBalanceNum(Node->right_) == -1) RightRotation(Node->right_);
      LeftRotation(Node);
    }
  }

  int GetBalanceNum(node* Node) const {
    return Node != nullptr
               ? GetHeightNum(Node->right_) - GetHeightNum(Node->left_)
               : 0;
  }

  int GetHeightNum(node* Node) const {
    return Node == nullptr ? -1 : Node->height_;
  }

  void SetHeight(node* Node) {
    if (Node != nullptr) {
      Node->height_ =
          std::max(GetHeightNum(Node->left_), GetHeightNum(Node->right_)) + 1;
    }
  }

  static node* GetMinNode(node* Node) {
    while (Node != nullptr && Node->left_ != nullptr) Node = Node->left_;
    return Node;
  }

  static node* GetMaxNode(node* Node) {
    while (Node != nullptr && Node->right_ != nullptr) Node = Node->right_;
    return Node;
  }

  bool RecursiveInsertion(node* Node, const Key& key, Value value,
                          bool allow_duplicates = false) {
    if (Node == nullptr) return false;

    bool inserted = false;
    if (key < Node->key_ || (allow_duplicates && key == Node->key_)) {
      if (Node->left_ == nullptr) {
        Node->left_ = new node(key, value, Node);
        inserted = true;
      } else {
        inserted =
            RecursiveInsertion(Node->left_, key, value, allow_duplicates);
      }
    } else if (key > Node->key_) {
      if (Node->right_ == nullptr) {
        Node->right_ = new node(key, value, Node);
        inserted = true;
      } else {
        inserted =
            RecursiveInsertion(Node->right_, key, value, allow_duplicates);
      }
    } else {
      // Если allow_duplicates == false, то дубликаты не добавляются
      inserted = false;
    }
    SetHeight(Node);
    Balancing(Node);
    return inserted;
  }

  node* RecursiveDeleting(node* Node, Key key) {
    if (Node == nullptr) return nullptr;

    if (key < Node->key_) {
      Node->left_ = RecursiveDeleting(Node->left_, key);
    } else if (key > Node->key_) {
      Node->right_ = RecursiveDeleting(Node->right_, key);
    } else {
      if (Node->left_ == nullptr || Node->right_ == nullptr) {
        node* temp = Node->left_ ? Node->left_ : Node->right_;
        if (temp == nullptr) {
          temp = Node;
          Node = nullptr;
        } else {
          *Node = *temp;
        }
        delete temp;
      } else {
        node* temp = GetMinNode(Node->right_);
        Node->key_ = temp->key_;
        Node->value_ = temp->value_;
        Node->right_ = RecursiveDeleting(Node->right_, temp->key_);
      }
    }

    if (Node == nullptr) return Node;

    SetHeight(Node);
    Balancing(Node);
    return Node;
  }

  size_t RecursiveSize(node* Node) const {
    return Node == nullptr
               ? 0
               : RecursiveSize(Node->left_) + RecursiveSize(Node->right_) + 1;
  }

  node* RecursiveSearch(node* Node, const Key& key) const {
    if (Node == nullptr || Node->key_ == key) return Node;
    if (key < Node->key_) return RecursiveSearch(Node->left_, key);
    return RecursiveSearch(Node->right_, key);
  }
};

}  // namespace s21

#endif  // SRC_AVL_TREE_H
