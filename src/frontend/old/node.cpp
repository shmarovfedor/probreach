//
// Created by fedor on 28/11/18.
//

#include "node.h"
#include <sstream>
#include <regex>

using namespace std;
//using namespace pdrh;

std::ostream &operator<<(std::ostream &os, const node &n)
{
  if (n.operands.size() > 1)
  {
    os << "(";
    for (size_t i = 0; i < n.operands.size() - 1; i++)
    {
      os << *n.operands.at(i);
      os << n.value;
    }
    os << *n.operands.at(n.operands.size() - 1) << ")";
  }
  else if (n.operands.size() == 1)
  {
    if (n.value == "-")
      os << "(" << n.value << *n.operands.front() << ")";
    else
      os << n.value << "(" << *n.operands.front() << ")";
  }
  else
    os << n.value;

  return os;
}

/**
 * Getting a string representation of the node in infix notation.
 *
 * @return node in infix notation as string.
 */
string node::to_infix()
{
  stringstream s;
  s << *this;
  return s.str();
}

/**
 * Getting a string representation of the node in prefix notation.
 *
 * @return node in prefix notation as string.
 */
string node::to_prefix()
{
  stringstream s;
  // checking whether n is an operation node
  if (this->operands.size() > 0)
  {
    s << "(" << this->value;
    for (node *op : this->operands)
    {
      s << op->to_prefix();
    }
    s << ")";
  }
  else
    s << " " << this->value;
  
  return s.str();
}

/**
 * Getting a string representation of the node in prefix notation with the fixed index.
 *
 * @param step - depth in the path.
 * @param index - an identifier.
 * @return node with fixed index as string.
 */
string node::to_prefix(int step, string index)
{
  stringstream s;
  // checking whether n is an operation node
  if (this->operands.size() > 0)
  {
    s << "(" << this->value;
    for (node *op : this->operands)
    {
      s << op->to_prefix(step, index);
    }
    s << ")";
  }
  else
  {
    // checking if the value is a constant or an identifier
    static const std::regex re(R"(^[+-]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$)");
    if (
      std::regex_match(this->value, re) || this->value == "true" ||
      this->value == "false")
    {
      s << " " << this->value;
    }
    else
    {
      s << " " << this->value << "_" << step << "_" << index;
    }
  }
  return s.str();
}

node* node::copy()
{
  node *copy = new node(this->value);
  for (node* op : this->operands) {
    node *op_copy = op->copy();
    copy->operands.push_back(op_copy);
  }
  return copy;
}

/**
 * Checking if the node is empty.
 *
 * @param n - node to check.
 * @return emptiness check result.
 */
bool node::is_empty()
{
  return this->value.empty() && this->operands.empty();
}
