//
// Created by fedor on 28/11/18.
//

#include "node.h"
#include <sstream>
#include <random>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace pdrh;

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
    // only performing a soft check here whether the value is digit or an identifier
    if (
      isdigit(this->value.front()) || this->value == "true" ||
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

/**
 * Copies the entire tree given its root
 *
 * @param copy - root node for the copy of the tree
 * @param origin - root node for the original tree
 */
void pdrh::copy_tree(node *&copy, node *origin)
{
  copy->value = origin->value;
  for (node *child : origin->operands)
  {
    node *copy_operand = new node;
    pdrh::copy_tree(copy_operand, child);
    copy->operands.push_back(copy_operand);
  }
}

/**
 * Creates a copy of the node.
 *
 * @param origin - original node
 * @return the copy of the node
 */
node *pdrh::copy_node(node *origin)
{
  node *copy = new node();
  copy_tree(copy, origin);
  return copy;
}

/**
 * Creating a string representation of the node in prefix notation
 * @param n - node to delete
 */
void pdrh::delete_node(node *n)
{
  for (node *op : n->operands)
  {
    delete_node(op);
  }
  delete n;
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

/**
 * Returns the first node matching the pattern (root->value == values[i]) and (expr).
 *
 * @param root - root of the tree.
 * @param res_node - resulting node.
 * @param values - list of values to check.
 */
void pdrh::get_first_node_by_value(
  node *root,
  node *res_node,
  vector<string> values)
{
  if (root->value == "=")
  {
    for (node *child : root->operands)
    {
      if (find(values.begin(), values.end(), child->value) != values.end())
      {
        *res_node = *root;
        root->value = "true";
        root->operands.clear();
      }
      else
      {
        pdrh::get_first_node_by_value(child, res_node, values);
      }
    }
  }
  else
  {
    for (node *child : root->operands)
    {
      pdrh::get_first_node_by_value(child, res_node, values);
    }
  }
}

/**
 * Returns the first node matching the pattern (root->value == values[i]) and (!expr)
 *
 * @param root - root of the tree.
 * @param values - resulting node.
 * @return
 */
node *pdrh::get_node_neg_by_value(node *root, vector<string> values)
{
  node *root_copy = new node();
  pdrh::copy_tree(root_copy, root);
  node *time_node = new node;
  pdrh::get_first_node_by_value(root_copy, time_node, values);
  if (time_node->is_empty())
    return NULL;
  // creating a negation node
  node *not_node = new node("not", {root_copy});
  // creating a resulting node
  node *res_node = new node("and", {time_node, not_node});
  return res_node;
}
