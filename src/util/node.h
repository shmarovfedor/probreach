//
// Created by fedor on 28/11/18.
//

#ifndef PROBREACH_NODE_H
#define PROBREACH_NODE_H

#include <iostream>
#include <map>
#include <vector>

// node of the tree of mathematical expression
class node
{
public:
  // either a name of operation or a value of the operand (const or identifier)
  std::string value;
  // vector is empty if the node is terminal and non-empty if the node is
  // operation node
  std::vector<node *> operands;

  // Create a "node" from a "string" value.
  node(std::string value) : value(value)
  {
  }

  // Create a "node" from a "double" value. A double is converted to its
  // string representation using the first 16 digit after the decimal point
  node(double v);

  node(const node &rhs) : value(rhs.value), operands(rhs.operands)
  {
  }

  node(const std::string value, const std::vector<node *> operands)
    : value(value), operands(operands)
  {
  }

  node()
  {
  }

  friend std::ostream &operator<<(std::ostream &os, const node &n);

  bool is_empty();

  std::string to_infix();
  std::string to_prefix();
  std::string to_prefix(int step, std::string index);
};

namespace pdrh
{
node *copy_node(node *);
void copy_tree(node *&, node *);
void delete_node(node *);

void get_first_node_by_value(node *, node *, std::vector<std::string>);
node *get_node_neg_by_value(node *, std::vector<std::string>);
} // namespace pdrh

#endif // PROBREACH_NODE_H
