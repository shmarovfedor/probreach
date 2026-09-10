//
// Created by fedor on 24/01/16.
//

#include "model.h"
#include <sstream>
#include <algorithm>
#include <set>
#include <iostream>

using namespace std;

model::type model::model_type;
vector<modet> model::modes;
vector<statet> model::init;
vector<statet> model::goal;
declarationst model::declarations;

// adding a variable
void model::push_var(string var, node *left, node *right)
{
  if (
    model::declarations.var_map.find(var) != model::declarations.var_map.cend())
  {
    stringstream s;
    s << "multiple declaration of \"" << var << "\"";
    throw invalid_argument(s.str());
  }
  else
  {
    model::declarations.var_map.insert(make_pair(var, make_pair(left, right)));
  }
}

void model::finalise()
{
  // extracting declarations and populating the maps
  for (auto it = model::declarations.decls.cbegin();
       it != model::declarations.decls.cend();
       ++it)
  {
    declarationt decl = it->second;
    if (decl.decl->value == "const_decl")
    {
      model::push_var(decl.sym, decl.decl->operands[0], decl.decl->operands[0]);
    }
    else if (decl.decl->value == "var_decl")
    {
      model::push_var(decl.sym, decl.decl->operands[0], decl.decl->operands[1]);
    }
    else if (decl.decl->value == "dist_decl")
    {
      node *decl_node = decl.decl->operands[0];
      if (decl_node->value == "dist_normal")
      {
        model::push_normal(
          decl.sym, decl_node->operands[0], decl_node->operands[1]);
      }
      else if (decl_node->value == "dist_uniform")
      {
        model::push_uniform(
          decl.sym, decl_node->operands[0], decl_node->operands[1]);
      }
      else if (decl_node->value == "dist_exp")
      {
        model::push_exp(
          decl.sym, decl_node->operands[0]);
      }
      else if (decl_node->value == "dist_discrete")
      {
        std::map<node*, node*> dd_pairs;
        for (node* op : decl_node->operands)
          dd_pairs.insert(make_pair(op->operands[0], op->operands[1]));
        
        model::push_dd(decl.sym, dd_pairs);
      }
    }
  }

  // collecting all variables for which an ode is defined
  std::set<std::string> flow_vars;
  for (modet m : model::modes)
    for (auto it : m.odes)
      flow_vars.insert(it.first);

  // creating par_map (i.e. explicit nondet parameters)
  for (auto it : model::declarations.var_map)
  {
    if (
      flow_vars.find(it.first) == flow_vars.cend() &&
      model::declarations.par_map.find(it.first) ==
        model::declarations.par_map.cend() &&
      model::declarations.rv_map.find(it.first) ==
        model::declarations.rv_map.cend() &&
      model::declarations.dd_map.find(it.first) ==
        model::declarations.dd_map.cend())
    {
      model::declarations.par_map.insert(make_pair(it.first, it.second));
    }
  }

  // adding equations for the parameters (nondet and random)
  for (size_t i = 0; i < model::modes.size(); ++i)
  {
    for (auto it : model::declarations.par_map)
      model::modes[i].odes.insert(make_pair(it.first, new node("0")));
    for (auto it : model::declarations.rv_map)
      model::modes[i].odes.insert(make_pair(it.first, new node("0")));
    for (auto it : model::declarations.dd_map)
      model::modes[i].odes.insert(make_pair(it.first, new node("0")));
  }

  // adding implicit resets
  for (size_t i = 0; i < model::modes.size(); ++i)
  {
    for (size_t j = 0; j < model::modes[i].jumps.size(); ++j)
    {
      for (auto it : model::modes[i].odes)
      {
        if (
          model::modes[i].jumps[j].reset.find(it.first) ==
          model::modes[i].jumps[j].reset.cend())
        {
          model::modes[i].jumps[j].reset.insert(
            make_pair(it.first, new node(it.first)));
        }
      }
    }
  }

  model::set_model_type();
}

// adding continuous random variable
void model::push_rv(string var, node *pdf, node *left, node *right, node *start)
{
  model::declarations.rv_map.insert(
    make_pair(var, make_tuple(pdf, left, right, start)));
}

// adding discrete random variable
void model::push_dd(string var, map<node *, node *> m)
{
  model::push_var(var, new node("-infty"), new node("infty"));
  model::declarations.dd_map.insert(make_pair(var, m));
}

void model::push_uniform(string var, node *a, node *b)
{
  model::push_var(var, a, b);
  model::push_rv(var, model::uniform_to_node(a, b), a, b, a);
  model::declarations.uniform.insert(make_pair(var, make_pair(a, b)));
}

void model::push_normal(string var, node *mu, node *sigma)
{
  model::push_var(var, new node("-infty"), new node("infty"));
  model::push_rv(
    var,
    model::normal_to_node(var, mu, sigma),
    new node("-infty"),
    new node("infty"),
    mu);
  model::declarations.normal.insert(make_pair(var, make_pair(mu, sigma)));
}

void model::push_exp(string var, node *lambda)
{
  model::push_var(var, new node("0"), new node("infty"));
  model::push_rv(
    var,
    model::exp_to_node(var, lambda),
    new node("0"),
    new node("infty"),
    new node("0"));
  model::declarations.exp.insert(make_pair(var, lambda));
}

// getting pointer to the mode by id
modet *model::get_mode(std::string id)
{
  for (size_t i = 0; i < model::modes.size(); i++)
  {
    if (model::modes.at(i).id == id)
    {
      return &model::modes.at(i);
    }
  }
  return NULL;
}

// getting all paths of length path_length between begin and end modes
vector<vector<modet *>>
model::get_paths(modet *begin, modet *end, int path_length)
{
  // initializing the set of paths
  vector<std::vector<modet *>> paths;
  vector<modet *> path;
  path.push_back(begin);
  // initializing the stack
  vector<vector<modet *>> stack;
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
      modet *cur_mode = path.back();
      // getting the successors of the mode
      vector<modet *> successors = model::get_successors(cur_mode);
      for (modet *suc_mode : successors)
      {
        // appending the successor the current paths
        vector<modet *> new_path = path;
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
vector<vector<modet *>> model::get_all_paths(int path_length)
{
  vector<vector<modet *>> res;
  for (statet i : model::init)
  {
    for (statet g : model::goal)
    {
      vector<vector<modet *>> paths = model::get_paths(
        model::get_mode(i.id), model::get_mode(g.id), path_length);
      res.insert(res.end(), paths.begin(), paths.end());
    }
  }
  return res;
}

vector<vector<modet *>> model::get_all_paths(int min_depth, int max_depth)
{
  vector<vector<modet *>> res;
  for (int i = min_depth; i <= max_depth; i++)
  {
    vector<vector<modet *>> paths = model::get_all_paths(i);
    res.insert(res.end(), paths.begin(), paths.end());
  }
  return res;
}

// getting successors of the mode m
vector<modet *> model::get_successors(modet *m)
{
  vector<modet *> res;
  for (jumpt j : m->jumps)
  {
    modet *tmp = model::get_mode(j.next_id);
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

node *model::uniform_to_node(node *a, node *b)
{
  node *minus_node = new node("+", {b, a});
  return new node("/", {new node("1"), minus_node});
}

node *model::normal_to_node(string var, node *mu, node *sigma)
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

node *model::exp_to_node(string var, node *lambda)
{
  node *times_node = new node("*", {lambda, new node(var)});
  node *unary_minus_node = new node("-", {times_node});
  node *exp_node = new node("exp", {unary_minus_node});
  return new node("*", {exp_node, lambda});
}

void model::set_model_type()
{
  if (
    model::declarations.rv_map.empty() && model::declarations.dd_map.empty() &&
    model::declarations.par_map.empty())
  {
    model::model_type = model::type::HA;
  }
  else if (model::declarations.par_map.empty())
  {
    model::model_type = model::type::PHA;
  }
  else
  {
    model::model_type = model::type::NPHA;
  }
}

// getting string representation of the model
string model::to_string()
{
  stringstream out;
  out << "MODEL TYPE: " << model::model_type << endl;
  out << "DECLARATIONS:" << endl;
  for (auto it = model::declarations.decls.cbegin();
       it != model::declarations.decls.cend();
       ++it)
  {
    out << "|   " << it->first << " : " << *(it->second.decl) << "\n";
  }
  out << "VARIABLES:" << endl;
  for (auto it = model::declarations.var_map.cbegin();
       it != model::declarations.var_map.cend();
       ++it)
  {
    out << "|   " << it->first << " [" << it->second.first->to_prefix() << ", "
        << it->second.second->to_prefix() << "]" << endl;
  }
  out << "PARAMETERS:" << endl;
  for (auto it = model::declarations.par_map.cbegin();
       it != model::declarations.par_map.cend();
       it++)
  {
    out << "|   " << it->first << " [" << it->second.first->to_prefix() << ", "
        << it->second.second->to_prefix() << "]" << endl;
  }
  out << "CONTINUOUS RANDOM VARIABLES:" << endl;
  for (auto it = model::declarations.rv_map.cbegin();
       it != model::declarations.rv_map.cend();
       it++)
  {
    out << "|   pdf(" << it->first << ") = " << *(get<0>(it->second)) << "  | "
        << get<1>(it->second)->to_prefix() << " |   "
        << get<2>(it->second)->to_prefix() << "    |   "
        << get<3>(it->second)->to_prefix() << endl;
  }
  out << "DISCRETE RANDOM VARIABLES:" << endl;
  for (auto it = model::declarations.dd_map.cbegin();
       it != model::declarations.dd_map.cend();
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
  for (modet m : model::modes)
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
    for (jumpt j : m.jumps)
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
  for (statet s : model::init)
  {
    out << "|   MODE: " << s.id << endl;
    out << "|   PROPOSITION: " << s.prop->to_prefix() << endl;
  }
  if (model::goal.size() > 0)
  {
    out << "GOAL:" << endl;
    for (statet s : model::goal)
    {
      out << "|   MODE: " << s.id << endl;
      out << "|   PROPOSITION: " << s.prop->to_prefix() << endl;
    }
  }
  return out.str();
}
