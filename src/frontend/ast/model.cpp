//
// Created by fedor on 24/01/16.
//

#include "model.h"
#include <sstream>
#include <algorithm>
#include <set>
#include <iostream>

using namespace std;

// adding a variable
void old::modelt::push_var(string var, node *left, node *right)
{
  if (
    declarations.var_map.find(var) != declarations.var_map.cend())
  {
    stringstream s;
    s << "multiple declaration of \"" << var << "\"";
    throw invalid_argument(s.str());
  }
  else
  {
    declarations.var_map.insert(make_pair(var, make_pair(left, right)));
  }
}

void old::modelt::finalise()
{
  // extracting declarations and populating the maps
  for (auto it = declarations.decls.cbegin();
       it != declarations.decls.cend();
       ++it)
  {
    declarationt decl = it->second;
    if (decl.decl->value == "const_decl")
    {
      push_var(decl.sym, decl.decl->operands[0], decl.decl->operands[0]);
    }
    else if (decl.decl->value == "var_decl")
    {
      push_var(decl.sym, decl.decl->operands[0], decl.decl->operands[1]);
    }
    else if (decl.decl->value == "dist_decl")
    {
      node *decl_node = decl.decl->operands[0];
      if (decl_node->value == "dist_normal")
      {
        push_normal(
          decl.sym, decl_node->operands[0], decl_node->operands[1]);
      }
      else if (decl_node->value == "dist_uniform")
      {
        push_uniform(
          decl.sym, decl_node->operands[0], decl_node->operands[1]);
      }
      else if (decl_node->value == "dist_exp")
      {
        push_exp(
          decl.sym, decl_node->operands[0]);
      }
      else if (decl_node->value == "dist_discrete")
      {
        std::map<node*, node*> dd_pairs;
        for (node* op : decl_node->operands)
          dd_pairs.insert(make_pair(op->operands[0], op->operands[1]));
        
        push_dd(decl.sym, dd_pairs);
      }
    }
  }

  // collecting all variables for which an ode is defined
  std::set<std::string> flow_vars;
  for (old::modet m : modes)
    for (auto it : m.odes)
      flow_vars.insert(it.first);

  // creating par_map (i.e. explicit nondet parameters)
  for (auto it : declarations.var_map)
  {
    if (
      flow_vars.find(it.first) == flow_vars.cend() &&
      declarations.par_map.find(it.first) ==
        declarations.par_map.cend() &&
      declarations.rv_map.find(it.first) ==
        declarations.rv_map.cend() &&
      declarations.dd_map.find(it.first) ==
        declarations.dd_map.cend())
    {
      declarations.par_map.insert(make_pair(it.first, it.second));
    }
  }

  // adding equations for the parameters (nondet and random)
  for (size_t i = 0; i < modes.size(); ++i)
  {
    for (auto it : declarations.par_map)
      modes[i].odes.insert(make_pair(it.first, new node("0")));
    for (auto it : declarations.rv_map)
      modes[i].odes.insert(make_pair(it.first, new node("0")));
    for (auto it : declarations.dd_map)
      modes[i].odes.insert(make_pair(it.first, new node("0")));
  }

  // adding implicit resets
  for (size_t i = 0; i < modes.size(); ++i)
  {
    for (size_t j = 0; j < modes[i].jumps.size(); ++j)
    {
      for (auto it : modes[i].odes)
      {
        if (
          modes[i].jumps[j].reset.find(it.first) ==
          modes[i].jumps[j].reset.cend())
        {
          modes[i].jumps[j].reset.insert(
            make_pair(it.first, new node(it.first)));
        }
      }
    }
  }

  set_model_type();
}

// adding continuous random variable
void old::modelt::push_rv(string var, node *pdf, node *left, node *right, node *start)
{
  declarations.rv_map.insert(
    make_pair(var, make_tuple(pdf, left, right, start)));
}

// adding discrete random variable
void old::modelt::push_dd(string var, map<node *, node *> m)
{
  push_var(var, new node("-infty"), new node("infty"));
  declarations.dd_map.insert(make_pair(var, m));
}

void old::modelt::push_uniform(string var, node *a, node *b)
{
  push_var(var, a, b);
  push_rv(var, uniform_to_node(a, b), a, b, a);
  declarations.uniform.insert(make_pair(var, make_pair(a, b)));
}

void old::modelt::push_normal(string var, node *mu, node *sigma)
{
  push_var(var, new node("-infty"), new node("infty"));
  push_rv(
    var,
    normal_to_node(var, mu, sigma),
    new node("-infty"),
    new node("infty"),
    mu);
  declarations.normal.insert(make_pair(var, make_pair(mu, sigma)));
}

void old::modelt::push_exp(string var, node *lambda)
{
  push_var(var, new node("0"), new node("infty"));
  push_rv(
    var,
    exp_to_node(var, lambda),
    new node("0"),
    new node("infty"),
    new node("0"));
  declarations.exp.insert(make_pair(var, lambda));
}

// getting pointer to the mode by id
old::modet *old::modelt::get_mode(std::string id)
{
  for (size_t i = 0; i < modes.size(); i++)
  {
    if (modes.at(i).id == id)
    {
      return &modes.at(i);
    }
  }
  return NULL;
}

// getting all paths of length path_length between begin and end modes
vector<vector<old::modet *>>
old::modelt::get_paths(old::modet *begin, old::modet *end, int path_length)
{
  // initializing the set of paths
  vector<std::vector<old::modet *>> paths;
  vector<old::modet *> path;
  path.push_back(begin);
  // initializing the stack
  vector<vector<old::modet *>> stack;
  stack.push_back(path);
  while (!stack.empty())
  {
    // getting the first paths from the set of paths
    path = stack.front();
    stack.erase(stack.cbegin());
    // checking if the correct path of the required length is found
    if ((path.back() == end) && (path.size() == path_length + 1))
    {
      paths.push_back(path);
    }
    // proceeding only if the length of the current path is ascending then the required length
    else if (path.size() < path_length + 1)
    {
      // getting the last mode in the path
      old::modet *cur_mode = path.back();
      // getting the successors of the mode
      vector<old::modet *> successors = get_successors(cur_mode);
      for (old::modet *suc_mode : successors)
      {
        // appending the successor the current paths
        vector<old::modet *> new_path = path;
        new_path.push_back(suc_mode);
        // pushing the new path to the set of the paths
        stack.push_back(new_path);
      }
    }
  }
  return paths;
}

// getting all paths of length path_length for
// all combinations of init and goal modes
vector<vector<old::modet *>> old::modelt::get_all_paths(int path_length)
{
  vector<vector<old::modet *>> res;
  for (old::statet i : init)
  {
    for (old::statet g : goal)
    {
      vector<vector<old::modet *>> paths = get_paths(
        get_mode(i.id), get_mode(g.id), path_length);
      res.insert(res.end(), paths.begin(), paths.end());
    }
  }
  return res;
}

vector<vector<old::modet *>> old::modelt::get_all_paths(int min_depth, int max_depth)
{
  vector<vector<old::modet *>> res;
  for (int i = min_depth; i <= max_depth; i++)
  {
    vector<vector<old::modet *>> paths = get_all_paths(i);
    res.insert(res.end(), paths.begin(), paths.end());
  }
  return res;
}

// getting successors of the mode m
vector<old::modet *> old::modelt::get_successors(old::modet *m)
{
  vector<old::modet *> res;
  for (old::jumpt j : m->jumps)
  {
    old::modet *tmp = get_mode(j.next_id);
    if (tmp != NULL)
    {
      res.push_back(tmp);
    }
    else
    {
      stringstream s;
      s << "mode \"" << j.next_id
        << "\" is not defined but appears in the jump: " << j.guard << " ==>  @"
        << j.next_id << endl;
      throw invalid_argument(s.str());
    }
  }
  return res;
}

node *old::modelt::uniform_to_node(node *a, node *b)
{
  node *minus_node = new node("+", {b, a});
  return new node("/", {new node("1"), minus_node});
}

node *old::modelt::normal_to_node(string var, node *mu, node *sigma)
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

node *old::modelt::exp_to_node(string var, node *lambda)
{
  node *times_node = new node("*", {lambda, new node(var)});
  node *unary_minus_node = new node("-", {times_node});
  node *exp_node = new node("exp", {unary_minus_node});
  return new node("*", {exp_node, lambda});
}

void old::modelt::set_model_type()
{
  if (
    declarations.rv_map.empty() && declarations.dd_map.empty() &&
    declarations.par_map.empty())
  {
    old::modelt::model_type = type::HA;
  }
  else if (declarations.par_map.empty())
  {
    old::modelt::model_type = type::PHA;
  }
  else
  {
    old::modelt::model_type = type::NPHA;
  }
}

// getting string representation of the model
string old::modelt::to_string()
{
  stringstream out;
  out << "MODEL TYPE: " << model_type << endl;
  out << "DECLARATIONS:" << endl;
  for (auto it = declarations.decls.cbegin();
       it != declarations.decls.cend();
       ++it)
  {
    out << "|   " << it->first << " : " << *(it->second.decl) << "\n";
  }
  out << "VARIABLES:" << endl;
  for (auto it = declarations.var_map.cbegin();
       it != declarations.var_map.cend();
       ++it)
  {
    out << "|   " << it->first << " [" << it->second.first->to_prefix() << ", "
        << it->second.second->to_prefix() << "]" << endl;
  }
  out << "PARAMETERS:" << endl;
  for (auto it = declarations.par_map.cbegin();
       it != declarations.par_map.cend();
       it++)
  {
    out << "|   " << it->first << " [" << it->second.first->to_prefix() << ", "
        << it->second.second->to_prefix() << "]" << endl;
  }
  out << "CONTINUOUS RANDOM VARIABLES:" << endl;
  for (auto it = declarations.rv_map.cbegin();
       it != declarations.rv_map.cend();
       it++)
  {
    out << "|   pdf(" << it->first << ") = " << *(get<0>(it->second)) << "  | "
        << get<1>(it->second)->to_prefix() << " |   "
        << get<2>(it->second)->to_prefix() << "    |   "
        << get<3>(it->second)->to_prefix() << endl;
  }
  out << "DISCRETE RANDOM VARIABLES:" << endl;
  for (auto it = declarations.dd_map.cbegin();
       it != declarations.dd_map.cend();
       it++)
  {
    out << "|   dd(" << it->first << ") = (";
    for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); it2++)
    {
      out << it2->first->to_prefix() << " : " << it2->second->to_prefix()
          << ", ";
    }
    out << ")" << endl;
  }
  out << "MODES:" << endl;
  for (old::modet m : modes)
  {
    out << "|   MODE: " << m.id << ";" << endl;
    out << "|   TIME DOMAIN: [" << m.time.first->to_prefix() << ", "
        << m.time.second->to_prefix() << "]" << endl;
    out << "|   INVARIANTS:" << endl;
    for (node *n : m.invts)
    {
      out << "|   |   " << n->to_prefix() << endl;
    }
    out << "|   ODES:" << endl;
    for (auto it = m.odes.cbegin(); it != m.odes.cend(); it++)
    {
      out << "|   |   d[" << it->first << "]/dt = " << it->second->to_prefix()
          << endl;
    }
    out << "|   JUMPS:" << endl;
    for (old::jumpt j : m.jumps)
    {
      out << "|   |   GUARD: " << j.guard->to_prefix() << endl;
      out << "|   |   SUCCESSOR: " << j.next_id << endl;
      out << "|   |   RESETS:" << endl;
      for (auto it = j.reset.cbegin(); it != j.reset.cend(); it++)
      {
        out << "|   |   |   " << it->first
            << "\' := " << it->second->to_prefix() << endl;
      }
    }
  }
  out << "INIT:" << endl;
  for (old::statet s : init)
  {
    out << "|   MODE: " << s.id << endl;
    out << "|   PROPOSITION: " << s.prop->to_prefix() << endl;
  }
  if (goal.size() > 0)
  {
    out << "GOAL:" << endl;
    for (old::statet s : goal)
    {
      out << "|   MODE: " << s.id << endl;
      out << "|   PROPOSITION: " << s.prop->to_prefix() << endl;
    }
  }
  return out.str();
}
