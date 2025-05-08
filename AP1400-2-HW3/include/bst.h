#ifndef BST_H
#define BST_H

#include <functional>
#include <ostream>

class BST {

public:
  class Node {
  public:
    Node(int value, Node *left, Node *right);
    Node();
    Node(const Node &node);

    friend std::ostream &operator<<(std::ostream &os, const Node &v);

    int value;
    Node *left;
    Node *right;
  };

  BST();
  BST(std::initializer_list<int> const &list);
  BST(const BST &other);
  BST(BST &&other) noexcept;

  BST &operator=(const BST &other);
  BST &operator=(BST &&other) noexcept;

  BST &operator++();
  BST operator++(int);

  Node *&get_root();
  void bfs(std::function<void(Node *&node)> func) const;
  size_t length() const;
  bool add_node(int value);
  Node **find_node(int value);
  Node **find_parrent(int value);
  Node **find_successor(int value); // SHOULD be predecessor
  bool delete_node(int value);
  ~BST();
  friend std::ostream &operator<<(std::ostream &os, const BST &v);

private:
  Node *root;
};
std::strong_ordering operator<=>(const BST::Node &v, int rhs);
std::strong_ordering operator<=>(int lhs, const BST::Node &v);
bool operator==(const BST::Node &v, int rhs);
bool operator==(int lhs, const BST::Node &v);

#endif // BST_H