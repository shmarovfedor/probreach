//
// Created by fedor on 12/06/18.
//

#include "node_utils.h"
#include <iomanip>
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

/**
 * Computes the value of the node provided as the first arguments at the point specified by the second argument.
 *
 * @param n - root node of the expression tree.
 * @param vals - map defining the point.
 * @return value of the node.
 */
double pdrh::node_to_double(node *n, std::map<std::string, double> vals)
{
  // terminal node
  if (n->operands.size() == 0)
  {
    // returning a value only if the variable from the node appears in the vector of values
    if (vals.find(n->value) != vals.end())
    {
      return vals[n->value];
    }
    // in case of infinity
    else if (n->value == "-infty")
    {
      return -numeric_limits<double>::max();
    }
    else if (n->value == "infty")
    {
      return numeric_limits<double>::max();
    }
    // in case of a constant
    double val;
    istringstream s(n->value);
    s >> val;
    return val;
  }
  // operation node
  else
  {
    if (n->value == "+")
    {
      // unary plus
      if (n->operands.size() == 1)
      {
        return node_to_double(n->operands.front(), vals);
      }
      // summation
      else if (n->operands.size() == 2)
      {
        return node_to_double(n->operands.front(), vals) +
               node_to_double(n->operands.back(), vals);
      }
    }
    else if (n->value == "-")
    {
      // unary minus
      if (n->operands.size() == 1)
      {
        return -node_to_double(n->operands.front(), vals);
      }
      // subtraction
      else if (n->operands.size() == 2)
      {
        return node_to_double(n->operands.front(), vals) -
               node_to_double(n->operands.back(), vals);
      }
    }
    else if (n->value == "*")
    {
      return node_to_double(n->operands.front(), vals) *
             node_to_double(n->operands.back(), vals);
    }
    else if (n->value == "/")
    {
      return node_to_double(n->operands.front(), vals) /
             node_to_double(n->operands.back(), vals);
    }
    else if (n->value == "^")
    {
      return std::pow(
        node_to_double(n->operands.front(), vals),
        node_to_double(n->operands.back(), vals));
    }
    else if (n->value == "sqrt")
    {
      return std::sqrt(node_to_double(n->operands.front(), vals));
    }
    else if (n->value == "abs")
    {
      return std::abs(node_to_double(n->operands.front(), vals));
    }
    else if (n->value == "exp")
    {
      return std::exp(node_to_double(n->operands.front(), vals));
    }
    else if (n->value == "log")
    {
      return std::log(node_to_double(n->operands.front(), vals));
    }
    else if (n->value == "sin")
    {
      return std::sin(node_to_double(n->operands.front(), vals));
    }
    else if (n->value == "cos")
    {
      return std::cos(node_to_double(n->operands.front(), vals));
    }
    else if (n->value == "tan")
    {
      return std::tan(node_to_double(n->operands.front(), vals));
    }
    else if (n->value == "asin")
    {
      return std::asin(node_to_double(n->operands.front(), vals));
    }
    else if (n->value == "acos")
    {
      return std::acos(node_to_double(n->operands.front(), vals));
    }
    else if (n->value == "atan")
    {
      return std::atan(node_to_double(n->operands.front(), vals));
    }
    if (n->value == "dist_normal")
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      double mean = node_to_double(n->operands[0], vals);
      double stddev = node_to_double(n->operands[1], vals);

      std::normal_distribution<> dist(mean, stddev);
      return dist(gen);
    }
    else if (n->value == "dist_uniform")
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      double left = node_to_double(n->operands[0], vals);
      double right = node_to_double(n->operands[1], vals);

      std::uniform_real_distribution<> dist(left, right);
      return dist(gen);
    }
    else if (n->value == "dist_gamma")
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      double a = node_to_double(n->operands[0], vals);
      double b = node_to_double(n->operands[1], vals);

      std::gamma_distribution<> dist(a, b);
      return dist(gen);
    }
    else if (n->value == "dist_exp")
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      double param = node_to_double(n->operands[0], vals);

      std::exponential_distribution<> dist(param);
      return dist(gen);
    }
    else if (n->value == "dist_discrete")
    {
      std::random_device rd;
      std::mt19937 gen(rd());
      vector<double> weights;
      for (node *op : n->operands)
      {
        weights.push_back(node_to_double(op->operands[1], vals));
      }

      std::discrete_distribution<int> dist(weights.begin(), weights.end());
      int i = dist(gen);
      return node_to_double(n->operands[i]->operands[0], vals);
    }
    else
    {
      cerr << "Unknown function \"" << n->value << "\"";
      exit(EXIT_FAILURE);
    }
  }
}

/**
 * Computes the value of the node.
 *
 * @param n - root node of the expression tree.
 * @return - value of the node.
 */
double pdrh::node_to_double(node *n)
{
  return node_to_double(n, std::map<string, double>());
}

/**
 * Evaluates the value of a predicate at the point. Throws an exception in case
 * if one of the terminal modes is not a number.
 *
 * @param n - predicate to be evaluated.
 * @param vals - point for which the evaluation is performed
 * @return
 */
bool pdrh::node_to_boolean(node *n, std::map<std::string, double> vals)
{
  // comparison operators
  if (n->value == ">=")
  {
    return node_to_double(n->operands.front(), vals) >=
           node_to_double(n->operands.back(), vals);
  }
  else if (n->value == ">")
  {
    return node_to_double(n->operands.front(), vals) >
           node_to_double(n->operands.back(), vals);
  }
  else if (n->value == "=")
  {
    return node_to_double(n->operands.front(), vals) ==
           node_to_double(n->operands.back(), vals);
  }
  else if (n->value == "<")
  {
    return node_to_double(n->operands.front(), vals) <
           node_to_double(n->operands.back(), vals);
  }
  else if (n->value == "<=")
  {
    return node_to_double(n->operands.front(), vals) <=
           node_to_double(n->operands.back(), vals);
  }
  else if (n->value == "and")
  {
    bool res = true;
    for (node *nd : n->operands)
    {
      res = res && node_to_boolean(nd, vals);
    }
    return res;
  }
  else if (n->value == "or")
  {
    bool res = false;
    for (node *nd : n->operands)
    {
      res = res || node_to_boolean(nd, vals);
    }
    return res;
  }
  else
  {
    cerr << "Unrecognised or unsupported operation \"" << n->value << "\"";
    exit(EXIT_FAILURE);
  }
}

/**
 * Returns true if zero-crossing happens between the left-handside and the right-handside points.
 *
 *
 * @param expr - expression to check.
 * @param left - left point.
 * @param right - right point.
 * @return the result of zero-crossing check
 */
bool pdrh::node_zero_crossing(
  node *expr,
  std::map<std::string, double> left,
  std::map<std::string, double> right)
{
  // comparison operators
  if (
    expr->value == ">=" || expr->value == ">" || expr->value == "=" ||
    expr->value == "<" || expr->value == "<=")
  {
    return (node_to_double(expr->operands.front(), left) -
            node_to_double(expr->operands.back(), left)) *
             (node_to_double(expr->operands.front(), right) -
              node_to_double(expr->operands.back(), right)) <
           0;
  }
  else if (expr->value == "and")
  {
    bool res = true;
    for (node *n : expr->operands)
    {
      res = res && node_zero_crossing(n, left, right);
    }
    return res;
  }
  else if (expr->value == "or")
  {
    bool res = true;
    for (node *n : expr->operands)
    {
      res = res || node_zero_crossing(n, left, right);
    }
    return res;
  }
  else
  {
    cerr << "Unrecognised or unsupported operation \"" << expr->value << "\"";
    exit(EXIT_FAILURE);
  }
}
