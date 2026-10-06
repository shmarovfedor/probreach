//
// Created by fedor on 24/01/16.
//

#include <sstream>
#include <algorithm>
#include <set>
#include <iostream>

#include "symbol_table.h"

using namespace std;

// adding a variable
void old::symbol_tablet::push_var(string var, node *left, node *right)
{
  if (var_map.find(var) != var_map.cend())
  {
    stringstream s;
    s << "multiple declaration of \"" << var << "\"";
    throw invalid_argument(s.str());
  }
  else
  {
    var_map.insert(make_pair(var, make_pair(left, right)));
  }
}

// adding continuous random variable
void old::symbol_tablet::push_rv(string var, node *pdf, node *left, node *right, node *start)
{
  rv_map.insert(
    make_pair(var, make_tuple(pdf, left, right, start)));
}

// adding discrete random variable
void old::symbol_tablet::push_dd(string var, map<node *, node *> m)
{
  push_var(var, new node("-infty"), new node("infty"));
  dd_map.insert(make_pair(var, m));
}

void old::symbol_tablet::push_uniform(string var, node *a, node *b)
{
  push_var(var, a, b);
  push_rv(var, uniform_to_node(a, b), a, b, a);
  uniform.insert(make_pair(var, make_pair(a, b)));
}

void old::symbol_tablet::push_normal(string var, node *mu, node *sigma)
{
  push_var(var, new node("-infty"), new node("infty"));
  push_rv(
    var,
    normal_to_node(var, mu, sigma),
    new node("-infty"),
    new node("infty"),
    mu);
  normal.insert(make_pair(var, make_pair(mu, sigma)));
}

void old::symbol_tablet::push_exp(string var, node *lambda)
{
  push_var(var, new node("0"), new node("infty"));
  push_rv(
    var,
    exp_to_node(var, lambda),
    new node("0"),
    new node("infty"),
    new node("0"));
  exp.insert(make_pair(var, lambda));
}

node *old::symbol_tablet::uniform_to_node(node *a, node *b)
{
  node *minus_node = new node("+", {b, a});
  return new node("/", {new node("1"), minus_node});
}

node *old::symbol_tablet::normal_to_node(string var, node *mu, node *sigma)
{
  node *power_node_1 = new node("^", {sigma, new node("2")});
  node *mult_node_1 = new node("*", {new node("2"), power_node_1});
  node *minus_node = new node("-", {new node(var), mu});
  node *power_node_2 = new node("^", {minus_node, new node("2")});
  node *divide_node_1 = new node("/", {power_node_2, mult_node_1});
  node *unary_minus_node = new node("-", {divide_node_1});
  node *exp_node = new node("exp", {unary_minus_node});
  node *mult_node_2 = new node("*", {new node("2"), new node("3.14159265359")});
  node *sqrt_node = new node("sqrt", {mult_node_2});
  node *mult_node_3 = new node("*", {sigma, sqrt_node});
  node *divide_node_2 = new node("/", {new node("1"), mult_node_3});
  return new node("*", {exp_node, divide_node_2});
}

node *old::symbol_tablet::exp_to_node(string var, node *lambda)
{
  node *times_node = new node("*", {lambda, new node(var)});
  node *unary_minus_node = new node("-", {times_node});
  node *exp_node = new node("exp", {unary_minus_node});
  return new node("*", {exp_node, lambda});
}
