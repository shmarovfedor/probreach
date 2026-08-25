//
// Created by fedor on 12/06/18.
//

#include "node_utils.h"
#include <iomanip>
#include "model.h"
#include <random>

using namespace std;

// throws exception in case if one of the terminal modes is not a number
// evaluates the value of arithmetic expression
bool pdrh::node_to_boolean(node *expr, vector<box> boxes)
{
  // comparison operators
  if (expr->value == ">=")
  {
    return pdrh::node_to_interval(expr->operands.front(), boxes) >=
           pdrh::node_to_interval(expr->operands.back(), boxes);
  }
  else if (expr->value == ">")
  {
    return pdrh::node_to_interval(expr->operands.front(), boxes) >
           pdrh::node_to_interval(expr->operands.back(), boxes);
  }
  else if (expr->value == "=")
  {
    return pdrh::node_to_interval(expr->operands.front(), boxes) ==
           pdrh::node_to_interval(expr->operands.back(), boxes);
  }
  else if (expr->value == "<")
  {
    return pdrh::node_to_interval(expr->operands.front(), boxes) <
           pdrh::node_to_interval(expr->operands.back(), boxes);
  }
  else if (expr->value == "<=")
  {
    return pdrh::node_to_interval(expr->operands.front(), boxes) <=
           pdrh::node_to_interval(expr->operands.back(), boxes);
  }
  else if (expr->value == "and")
  {
    bool res = true;
    for (node *n : expr->operands)
    {
      res = res && pdrh::node_to_boolean(n, boxes);
    }
    return res;
  }
  else if (expr->value == "or")
  {
    bool res = true;
    for (node *n : expr->operands)
    {
      res = res || pdrh::node_to_boolean(n, boxes);
    }
    return res;
  }
  else
  {
    cerr << "Unrecognised or unsupported operation \"" << expr->value << "\"";
    exit(EXIT_FAILURE);
  }
}

// throws exception in case if one of the terminal modes is not a number
// evaluates the value of arithmetic expression
bool pdrh::check_zero_crossing(
  node *expr,
  vector<box> boxes,
  box first,
  box last)
{
  // comparison operators
  if (
    expr->value == ">=" || expr->value == ">" || expr->value == "=" ||
    expr->value == "<" || expr->value == "<=")
  {
    return (pdrh::node_to_interval(expr->operands.front(), {boxes, first}) -
            pdrh::node_to_interval(expr->operands.back(), {boxes, first})) *
             (pdrh::node_to_interval(
                expr->operands.front(), {boxes, last}) -
              pdrh::node_to_interval(
                expr->operands.back(), {boxes, last})) <
           0;
  }
  else if (expr->value == "and")
  {
    bool res = true;
    for (node *n : expr->operands)
    {
      res = res && pdrh::check_zero_crossing(n, boxes, first, last);
    }
  }
  else if (expr->value == "or")
  {
    bool res = true;
    for (node *n : expr->operands)
    {
      res = res || pdrh::check_zero_crossing(n, boxes, first, last);
    }
  }
  else
  {
    cerr << "Unrecognised or unsupported operation \"" << expr->value << "\"";
    exit(EXIT_FAILURE);
  }
}

// throws exception in case if one of the terminal modes is not a number
// evaluates the value of arithmetic expression
capd::interval pdrh::node_to_interval(node *expr, vector<box> boxes)
{
  // terminal node
  if (expr->operands.size() == 0)
  {
    for (box b : boxes)
    {
      map<string, capd::interval> b_map = b.get_map();
      if (b_map.find(expr->value) != b_map.end())
      {
        return b_map[expr->value];
      }
    }
    if (expr->value == "-infty")
    {
      return capd::interval(
        -numeric_limits<double>::max(), -numeric_limits<double>::max());
    }
    else if (expr->value == "infty")
    {
      return capd::interval(
        numeric_limits<double>::max(), numeric_limits<double>::max());
    }
    return capd::interval(expr->value, expr->value);
  }
  else if (expr->operands.size() > 2)
  {
    exit(EXIT_FAILURE);
  }
  else
  {
    if (expr->value == "+")
    {
      if (expr->operands.size() == 1)
      {
        return pdrh::node_to_interval(expr->operands.front(), boxes);
      }
      else if (expr->operands.size() == 2)
      {
        return pdrh::node_to_interval(expr->operands.front(), boxes) +
               pdrh::node_to_interval(expr->operands.back(), boxes);
      }
    }
    else if (expr->value == "-")
    {
      if (expr->operands.size() == 1)
      {
        return capd::interval(-1.0) *
               pdrh::node_to_interval(expr->operands.front(), boxes);
      }
      else if (expr->operands.size() == 2)
      {
        return pdrh::node_to_interval(expr->operands.front(), boxes) -
               pdrh::node_to_interval(expr->operands.back(), boxes);
      }
    }
    else if (expr->value == "*")
    {
      return pdrh::node_to_interval(expr->operands.front(), boxes) *
             pdrh::node_to_interval(expr->operands.back(), boxes);
    }
    else if (expr->value == "/")
    {
      return pdrh::node_to_interval(expr->operands.front(), boxes) /
             pdrh::node_to_interval(expr->operands.back(), boxes);
    }
    else if (expr->value == "^")
    {
      return capd::intervals::power(
        pdrh::node_to_interval(expr->operands.front(), boxes),
        pdrh::node_to_interval(expr->operands.back(), boxes));
    }
    else if (expr->value == "sqrt")
    {
      return capd::intervals::sqrt(
        pdrh::node_to_interval(expr->operands.front(), boxes));
    }
    else if (expr->value == "abs")
    {
      return capd::intervals::iabs(
        pdrh::node_to_interval(expr->operands.front(), boxes));
    }
    else if (expr->value == "exp")
    {
      return capd::intervals::exp(
        pdrh::node_to_interval(expr->operands.front(), boxes));
    }
    else if (expr->value == "log")
    {
      return capd::intervals::log(
        pdrh::node_to_interval(expr->operands.front(), boxes));
    }
    else if (expr->value == "sin")
    {
      return capd::intervals::sin(
        pdrh::node_to_interval(expr->operands.front(), boxes));
    }
    else if (expr->value == "cos")
    {
      return capd::intervals::cos(
        pdrh::node_to_interval(expr->operands.front(), boxes));
    }
    else if (expr->value == "tan")
    {
      return capd::intervals::tan(
        pdrh::node_to_interval(expr->operands.front(), boxes));
    }
    else if (expr->value == "asin")
    {
      return capd::intervals::asin(
        pdrh::node_to_interval(expr->operands.front(), boxes));
    }
    else if (expr->value == "acos")
    {
      return capd::intervals::acos(
        pdrh::node_to_interval(expr->operands.front(), boxes));
    }
    else if (expr->value == "atan")
    {
      return capd::intervals::atan(
        pdrh::node_to_interval(expr->operands.front(), boxes));
    }
    else
    {
      cerr << "Unknown function \"" << expr->value << "\"";
      exit(EXIT_FAILURE);
    }
  }
}

// throws exception in case if one of the terminal modes is not a number
// evaluates the value of arithmetic expression
capd::interval pdrh::node_to_interval(node *expr)
{
  return pdrh::node_to_interval(expr, {box()});
}

node *pdrh::box_to_node(box b)
{
  node *res = new node();
  res->value = "and";
  map<string, capd::interval> b_map = b.get_map();
  for (auto it = b_map.begin(); it != b_map.end(); it++)
  {
    // adding left node
    node *node_left = new node();
    node_left->value = ">=";
    node_left->operands.push_back(new node(it->first));
    stringstream ss;
    ss << std::setprecision(16) << it->second.leftBound();
    node_left->operands.push_back(new node(ss.str()));
    res->operands.push_back(node_left);
    // adding right node
    node *node_right = new node();
    node_right->value = "<=";
    node_right->operands.push_back(new node(it->first));
    ss.str("");
    ss << std::setprecision(16) << it->second.rightBound();
    node_right->operands.push_back(new node(ss.str()));
    res->operands.push_back(node_right);
  }
  return res;
}

// domain of nondeterministic parameters
box pdrh::get_nondet_domain()
{
  map<std::string, capd::interval> m;
  for (auto it = pdrh::par_map.cbegin(); it != pdrh::par_map.cend(); it++)
  {
    m.insert(make_pair(
      it->first,
      capd::interval(
        pdrh::node_to_interval(it->second.first).leftBound(),
        pdrh::node_to_interval(it->second.second).rightBound())));
  }
  return box(m);
}

// domain of system variables
box pdrh::get_domain()
{
  map<std::string, capd::interval> m;
  for (auto it = pdrh::var_map.cbegin(); it != pdrh::var_map.cend(); it++)
  {
    m.insert(make_pair(
      it->first,
      capd::interval(
        pdrh::node_to_interval(it->second.first).leftBound(),
        pdrh::node_to_interval(it->second.second).rightBound())));
  }
  return box(m);
}

// only the first initial state is taken
box pdrh::init_to_box(vector<box> boxes)
{
  node *init_node = pdrh::init.front().prop;
  if (init_node->value != "and")
  {
    cerr << "Invalid initial state format: " << pdrh::init.front() << endl;
    exit(EXIT_FAILURE);
  }

  map<string, capd::interval> b_map;
  for (node *n : init_node->operands)
  {
    if (n->value != "=")
    {
      cerr << "Invalid assignment in the initial state: " << n << endl;
      exit(EXIT_FAILURE);
    }

    if (
      (pdrh::var_map.find(n->operands.front()->value) != pdrh::var_map.end()) ||
      (pdrh::par_map.find(n->operands.front()->value) != pdrh::par_map.end()) ||
      (pdrh::rv_map.find(n->operands.front()->value) != pdrh::rv_map.end()) ||
      (pdrh::dd_map.find(n->operands.front()->value) != pdrh::dd_map.end()))
    {
      b_map.insert(make_pair(
        n->operands.front()->value,
        pdrh::node_to_interval(n->operands.back(), boxes)));
    }
    else if (
      (pdrh::var_map.find(n->operands.back()->value) != pdrh::var_map.end()) ||
      (pdrh::par_map.find(n->operands.back()->value) != pdrh::par_map.end()) ||
      (pdrh::rv_map.find(n->operands.back()->value) != pdrh::rv_map.end()) ||
      (pdrh::dd_map.find(n->operands.back()->value) != pdrh::dd_map.end()))
    {
      b_map.insert(make_pair(
        n->operands.back()->value,
        pdrh::node_to_interval(n->operands.front(), boxes)));
    }
  }
  return box(b_map);
}

