#include "bst.h"

#include <iomanip>
#include <queue>

BST::Node::Node(int value, Node *left = nullptr, Node *right = nullptr)
    : value(value), left(left), right(right) {}

BST::Node::Node() : Node(0) {}

BST::Node::Node(const Node &node) : Node(node.value, node.left, node.right) {}

BST::BST() : root(nullptr) {}

BST::BST(std::initializer_list<int> const &list) : BST() {
  for (int v : list) {
    add_node(v);
  }
}

BST::BST(const BST &other) : BST() {
  other.bfs([this](Node *&x) { this->add_node(x->value); });
}

BST::BST(BST &&other) noexcept : root(other.root) { other.root = nullptr; }

BST &BST::operator=(const BST &other) {
  if (this == &other) {
    return *this;
  }
  this->~BST();
  this->root = nullptr;
  other.bfs([this](Node *&x) { this->add_node(x->value); });
  return *this;
}

BST &BST::operator=(BST &&other) noexcept {
  if (this == &other) {
    return *this;
  }
  this->~BST();
  this->root = other.root;
  other.root = nullptr;
  return *this;
}

BST BST::operator++(int) {
  BST res = *this;
  ++(*this);
  return res;
}

BST &BST::operator++() {
  bfs([](Node *&x) { x->value += 1; });
  return *this;
}

std::ostream &operator<<(std::ostream &os, const BST::Node &v) {
  os << std::setw(17) << std::left << &v;
  os << "=> value:";
  os << std::setw(10) << std::left << v.value;
  os << "left:";
  os << std::setw(16) << std::left << v.left;
  os << "right:";
  os << std::setw(16) << std::left << v.right;
  return os;
}

std::strong_ordering operator<=>(const BST::Node &v, int rhs) {
  return v.value <=> rhs;
}
std::strong_ordering operator<=>(int lhs, const BST::Node &v) {
  return lhs <=> v.value;
}
bool operator==(const BST::Node &v, int rhs) { return v.value == rhs; }
bool operator==(int lhs, const BST::Node &v) { return lhs == v.value; }

BST::Node *&BST::get_root() { return this->root; }

void BST::bfs(std::function<void(Node *&node)> func) const {
  std::queue<BST::Node *> q;
  if (this->root) {
    q.push(this->root);
  }

  while (!q.empty()) {
    auto u = q.front();
    q.pop();
    func(u);
    if (u->left) {
      q.push(u->left);
    }
    if (u->right) {
      q.push(u->right);
    }
  }
}

size_t BST::length() const {
  size_t res = 0;
  auto counter = [&res](Node *&node) { res += 1; };
  bfs(counter);
  return res;
}

bool BST::add_node(int value) {
  bool fail = false;
  auto insert = [&](auto &&self, BST::Node *u) -> Node * {
    if (!u) {
      return new Node(value);
    }

    if (u->value == value) {
      fail = true;
    } else if (u->value > value) {
      u->left = self(self, u->left);
    } else {
      u->right = self(self, u->right);
    }

    return u;
  };

  root = insert(insert, root);
  return !fail;
}

BST::Node **BST::find_node(int value) {
  auto find = [&](auto &&self, Node **u) -> Node ** {
    if (!*u) {
      return nullptr;
    }
    if ((*u)->value == value) {
      return u;
    } else if ((*u)->value > value) {
      return self(self, &((*u)->left));
    } else {
      return self(self, &((*u)->right));
    }
  };
  return find(find, &root);
}

BST::Node **BST::find_parrent(int value) {
  auto find = [&](auto &&self, Node **u) -> Node ** {
    if (!*u) {
      return nullptr;
    }

    if ((*u)->left && (*u)->left->value == value) {
      return u;
    }

    if ((*u)->right && (*u)->right->value == value) {
      return u;
    }

    if ((*u)->value == value) {
      return nullptr;
    } else if ((*u)->value > value) {
      return self(self, &((*u)->left));
    } else {
      return self(self, &((*u)->right));
    }
  };
  return find(find, &root);
}

BST::Node **BST::find_successor(int value) { // SHOULD be predecessor
  auto find_right_most = [&](auto &&self, Node **u) -> Node ** {
    if (!u || !(*u)) {
      return nullptr;
    }
    if ((*u)->right) {
      return self(self, &((*u)->right));
    } else {
      return u;
    }
  };

  auto x = find_node(value);

  if (!x) {
    return nullptr;
  }

  if (*x && (*x)->left) {
    return find_right_most(find_right_most, &(*x)->left);
  }

  Node **res = nullptr;
  auto func = [&](Node *&y) {
    if ((*y).value < value) {
      res = &y;
    }
  };
  bfs(func);
  return res;
}

bool BST::delete_node(int value) {
  auto x = find_node(value);
  auto y = find_parrent(value);
  if (!x) {
    return false;
  }

  auto cur = *x;

  if (!cur->left && !cur->right) {
    if (y) {
      if ((*y)->left && (*y)->left->value == value) {
        (*y)->left = nullptr;
      } else {
        (*y)->right = nullptr;
      }
    } else {
      root = nullptr;
    }
    delete cur;
  } else if (!cur->left || !cur->right) {
    Node *child = cur->left ? cur->left : cur->right;
    if (y) {
      if ((*y)->left && (*y)->left->value == value) {
        (*y)->left = child;
      } else {
        (*y)->right = child;
      }
    } else {
      root = child;
    }
    delete cur;
  } else {
    auto z = find_successor(value);
    auto k = find_parrent((*z)->value);
    std::swap(cur->value, (*z)->value);
    if ((*k)->left && (*k)->left->value == value) {
      (*k)->left = nullptr;
    } else {
      (*k)->right = nullptr;
    }
    delete *z;
  }
  return true;
}

std::ostream &operator<<(std::ostream &os, const BST &v) {
  os << " " << std::string(80, '*') << '\n';
  auto print = [&](BST::Node *&node) { os << " " << *node << "\n"; };
  v.bfs(print);
  os << " binary search tree size: " << v.length() << "\n";
  os << " " << std::string(80, '*') << '\n';
  return os;
}

BST::~BST() {
  std::vector<Node *> nodes;
  bfs([&nodes](BST::Node *&node) { nodes.push_back(node); });
  for (auto &node : nodes)
    delete node;
}