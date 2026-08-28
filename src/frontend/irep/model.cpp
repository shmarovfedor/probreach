//
// Created by fedor on 24/01/16.
//

#include "model.h"
#include <string.h>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <capd/capdlib.h>
#include <capd/intervals/lib.h>

using namespace std;

model::type model::model_type;
pair<node *, node *> model::time;
map<string, tuple<node *, node *, node *, node *>> model::rv_map;
map<string, map<node *, node *>> model::dd_map;
map<string, pair<node *, node *>> model::var_map;
map<string, pair<node *, node *>> model::par_map;

vector<model::mode> model::modes;
vector<model::state> model::init;
vector<model::state> model::goal;

map<string, pair<node *, node *>> model::distribution::uniform;
map<string, pair<node *, node *>> model::distribution::normal;
map<string, node *> model::distribution::exp;
map<string, pair<node *, node *>> model::distribution::gamma;

// adding a variable
void model::push_var(string var, node *left, node *right)
{
  if (model::var_map.find(var) != model::var_map.cend())
  {
    stringstream s;
    s << "multiple declaration of \"" << var << "\"";
    throw invalid_argument(s.str());
  }
  else
  {
    model::var_map.insert(make_pair(var, make_pair(left, right)));
  }
}

// adding time bounds
void model::push_time_bounds(node *left, node *right)
{
  model::time = make_pair(left, right);
}

// adding invariant
void model::push_invt(model::mode &m, node *invt)
{
  m.invts.push_back(invt);
}

// adding mode
void model::push_mode(model::mode m)
{
  vector<string> extra_vars = model::get_keys_diff(model::var_map, m.flow_map);
  for (string var : extra_vars)
  {
    m.flow_map.insert(make_pair(var, model::var_map[var]));
    m.odes.insert(make_pair(var, new node("0")));
    if (model::var_map[var].first != model::var_map[var].second)
    {
      // adding this variable to the list of parameters if it is not there yet,
      // if it is not a continuous or discrete random variable and
      // if its domain is an interval of length descending than 0.
      // There might be a problem as the length of the interval
      // is always different from 0 due to overapproximation of
      // the interval arithmetics
      if (
        model::par_map.find(var) == model::par_map.cend() &&
        model::rv_map.find(var) == model::rv_map.cend() &&
        model::dd_map.find(var) == model::dd_map.cend() &&
        model::var_map[var].first != model::var_map[var].second)
      {
        bool insert_flag = true;
        for (model::mode md : model::modes)
        {
          if (md.flow_map.find(var) != md.flow_map.cend())
          {
            insert_flag = false;
            break;
          }
        }
        if (insert_flag)
        {
          model::par_map.insert(make_pair(var, model::var_map[var]));
        }
      }
    }
  }
  model::modes.push_back(m);
}

// adding ode to the mode
void model::push_ode(model::mode &m, string var, node *ode)
{
  if (model::var_map.find(var) != model::var_map.cend())
  {
    if (m.flow_map.find(var) == m.flow_map.cend())
    {
      m.flow_map.insert(make_pair(var, model::var_map[var]));
      m.odes.insert(make_pair(var, ode));
      // removing a variable from the parameter list if there is an ode defined for it
      if (model::par_map.find(var) != model::par_map.cend())
      {
        model::par_map.erase(var);
      }
    }
    else
    {
      stringstream s;
      s << "ode for the variable \"" << var << "\" was already declared above";
      throw invalid_argument(s.str());
    }
  }
  else
  {
    stringstream s;
    s << "variable \"" << var
      << "\" appears in the flow but it was not declared";
    throw invalid_argument(s.str());
  }
}

// adding a reset
void model::push_reset(
  model::mode &m,
  model::mode::jump &j,
  string var,
  node *expr)
{
  // implement error check
  j.reset.insert(make_pair(var, expr));
}

// adding a jump
void model::push_jump(model::mode &m, mode::jump j)
{
  m.jumps.push_back(j);
}

// adding init
void model::push_init(vector<model::state> s)
{
  model::init = s;
}

// adding goal
void model::push_goal(vector<model::state> s)
{
  model::goal = s;
}

// adding continuous random variable
void model::push_rv(string var, node *pdf, node *left, node *right, node *start)
{
  model::rv_map.insert(make_pair(var, make_tuple(pdf, left, right, start)));
}

// adding discrete random variable
void model::push_dd(string var, map<node *, node *> m)
{
  model::dd_map.insert(make_pair(var, m));
}

// checking if the variable exists
bool model::var_exists(string var)
{
  return (model::var_map.find(var) != model::var_map.cend());
}

// getting pointer to the mode by id
model::mode *model::get_mode(int id)
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

model::mode::jump model::mode::get_jump(int id)
{
  for (size_t i = 0; i < this->jumps.size(); i++)
  {
    if (this->jumps.at(i).next_id == id)
    {
      return this->jumps.at(i);
    }
  }
}

// getting all paths of length path_length between begin and end modes
vector<vector<model::mode *>>
model::get_paths(model::mode *begin, model::mode *end, int path_length)
{
  // initializing the set of paths
  vector<std::vector<model::mode *>> paths;
  vector<model::mode *> path;
  path.push_back(begin);
  // initializing the stack
  vector<vector<model::mode *>> stack;
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
      model::mode *cur_mode = path.back();
      // getting the successors of the mode
      vector<model::mode *> successors = model::get_successors(cur_mode);
      for (model::mode *suc_mode : successors)
      {
        // appending the successor the current paths
        vector<model::mode *> new_path = path;
        new_path.push_back(suc_mode);
        // pushing the new path to the set of the paths
        stack.push_back(new_path);
      }
    }
  }
  return paths;
}

// comparing two paths alphabetically
bool compare_paths_ascending(vector<model::mode *> lhs, vector<model::mode *> rhs)
{
  if (lhs.size() < rhs.size())
  {
    return true;
  }
  else if (lhs.size() > rhs.size())
  {
    return false;
  }
  else
  {
    stringstream s;
    for (model::mode *m : lhs)
    {
      s << m->id;
    }
    string lstring = s.str();
    s.str("");
    for (model::mode *m : rhs)
    {
      s << m->id;
    }
    if (lstring.compare(s.str()) <= 0)
    {
      return true;
    }
    return false;
  }
}

// getting all paths of length path_length for all combinations of init and goal modes
vector<vector<model::mode *>> model::get_all_paths(int path_length)
{
  vector<vector<model::mode *>> res;
  for (model::state i : model::init)
  {
    for (model::state g : model::goal)
    {
      vector<vector<model::mode *>> paths = model::get_paths(
        model::get_mode(i.id), model::get_mode(g.id), path_length);
      res.insert(res.end(), paths.begin(), paths.end());
    }
  }
  // sorting all paths if the ascending order
  sort(res.begin(), res.end(), compare_paths_ascending);
  return res;
}

vector<vector<model::mode *>> model::get_all_paths(int min_depth, int max_depth)
{
  vector<vector<model::mode *>> res;
  for (int i = min_depth; i <= max_depth; i++)
  {
    vector<vector<model::mode *>> paths = model::get_all_paths(i);
    res.insert(res.end(), paths.begin(), paths.end());
  }
  // sorting all paths if the ascending order
  sort(res.begin(), res.end(), compare_paths_ascending);
  return res;
}

// getting successors of the mode m
vector<model::mode *> model::get_successors(model::mode *m)
{
  vector<model::mode *> res;
  for (model::mode::jump j : m->jumps)
  {
    model::mode *tmp = model::get_mode(j.next_id);
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

// getting initial modes
vector<model::mode *> model::get_init_modes()
{
  vector<model::mode *> res;
  for (model::state st : model::init)
  {
    model::mode *tmp = model::get_mode(st.id);
    if (tmp != NULL)
    {
      res.push_back(tmp);
    }
    else
    {
      stringstream s;
      s << "mode \"" << st.id << "\" is not defined but appears in the init";
      throw invalid_argument(s.str());
    }
  }
  return res;
}

// getting goal modes
vector<model::mode *> model::get_goal_modes()
{
  vector<model::mode *> res;
  for (model::state st : model::goal)
  {
    model::mode *tmp = model::get_mode(st.id);
    if (tmp != NULL)
    {
      res.push_back(tmp);
    }
    else
    {
      stringstream s;
      s << "mode \"" << st.id << "\" is not defined but appears in the goal";
      throw invalid_argument(s.str());
    }
  }
  return res;
}

// getting a difference of the key sets of two maps
vector<string> model::get_keys_diff(
  map<string, pair<node *, node *>> left,
  map<string, pair<node *, node *>> right)
{
  vector<string> res;
  for (auto it = left.cbegin(); it != left.cend(); it++)
  {
    if (right.find(it->first) == right.cend())
    {
      res.push_back(it->first);
    }
  }
  return res;
}

// getting string representation of the model
string model::model_to_string()
{
  stringstream out;
  out << "MODEL TYPE: " << model::model_type << endl;
  out << "VARIABLES:" << endl;
  for (auto it = model::var_map.cbegin(); it != model::var_map.cend(); it++)
  {
    out << "|   " << it->first << " [" << it->second.first->to_prefix() << ", "
        << it->second.second->to_prefix() << "]" << endl;
  }
  out << "PARAMETERS:" << endl;
  for (auto it = model::par_map.cbegin(); it != model::par_map.cend(); it++)
  {
    out << "|   " << it->first << " [" << it->second.first->to_prefix() << ", "
        << it->second.second->to_prefix() << "]" << endl;
  }
  out << "CONTINUOUS RANDOM VARIABLES:" << endl;
  for (auto it = model::rv_map.cbegin(); it != model::rv_map.cend(); it++)
  {
    out << "|   pdf(" << it->first << ") = " << get<0>(it->second) << "  | "
        << get<1>(it->second)->to_prefix() << " |   "
        << get<2>(it->second)->to_prefix() << "    |   "
        << get<3>(it->second)->to_prefix() << endl;
  }
  out << "DISCRETE RANDOM VARIABLES:" << endl;
  for (auto it = model::dd_map.cbegin(); it != model::dd_map.cend(); it++)
  {
    out << "|   dd(" << it->first << ") = (";
    for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); it2++)
    {
      cout << it2->first->to_prefix() << " : " << it2->second->to_prefix()
           << endl;
      out << it2->first->to_prefix() << " : " << it2->second->to_prefix()
          << ", ";
    }
    out << ")" << endl;
  }
  out << "MODES:" << endl;
  for (model::mode m : model::modes)
  {
    out << "|   MODE: " << m.id << ";" << endl;
    out << "|   TIME DOMAIN: [" << m.time.first->to_prefix() << ", "
        << m.time.second->to_prefix() << "]" << endl;
    out << "|   INVARIANTS:" << endl;
    for (node *n : m.invts)
    {
      out << "|   |   " << n->to_prefix() << endl;
    }
    out << "|   FLOW_MAP:" << endl;
    for (auto it = m.flow_map.cbegin(); it != m.flow_map.cend(); it++)
    {
      out << "|   " << it->first << " "
          << " [" << it->second.first->to_prefix() << ", "
          << it->second.second->to_prefix() << "]" << endl;
    }
    out << "|   ODES:" << endl;
    for (auto it = m.odes.cbegin(); it != m.odes.cend(); it++)
    {
      out << "|   |   d[" << it->first << "]/dt = " << it->second->to_prefix()
          << endl;
    }
    out << "|   JUMPS:" << endl;
    for (model::mode::jump j : m.jumps)
    {
      out << "|   |   GUARD: " << j.guard->to_prefix() << endl;
      out << "|   |   SUCCESSOR: " << j.next_id << endl;
      out << "|   |   RESETS:" << endl;
      for (auto it = j.reset.cbegin(); it != j.reset.cend(); it++)
      {
        out << "|   |   |   " << it->first << " := " << it->second->to_prefix()
            << endl;
      }
      out << "|   |   RESETS RV:" << endl;
      for (auto it = j.reset_rv.cbegin(); it != j.reset_rv.cend(); it++)
      {
        out << "|   |   " << get<0>(it->second) << "   |   "
            << get<1>(it->second) << "  | " << get<2>(it->second)->to_prefix()
            << " |   " << get<3>(it->second)->to_prefix() << "    |   "
            << get<4>(it->second)->to_prefix() << endl;
      }
      out << "|   |   RESETS DD:" << endl;
      for (auto it = j.reset_dd.cbegin(); it != j.reset_dd.cend(); it++)
      {
        out << "|   |   |   dd(" << it->first << ") = (";
        for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); it2++)
        {
          out << it2->first->to_prefix() << " : " << it2->second->to_prefix()
              << ", ";
        }
        out << ")" << endl;
      }
    }
  }
  out << "INIT:" << endl;
  for (model::state s : model::init)
  {
    out << "|   MODE: " << s.id << endl;
    out << "|   PROPOSITION: " << s.prop->to_prefix() << endl;
  }
  if (model::goal.size() > 0)
  {
    out << "GOAL:" << endl;
    for (model::state s : model::goal)
    {
      out << "|   MODE: " << s.id << endl;
      out << "|   PROPOSITION: " << s.prop->to_prefix() << endl;
    }
  }
  return out.str();
}

void model::distribution::push_uniform(string var, node *a, node *b)
{
  model::distribution::uniform.insert(make_pair(var, make_pair(a, b)));
}

void model::distribution::push_normal(string var, node *mu, node *sigma)
{
  model::distribution::normal.insert(make_pair(var, make_pair(mu, sigma)));
}

void model::distribution::push_gamma(string var, node *a, node *b)
{
  model::distribution::gamma.insert(make_pair(var, make_pair(a, b)));
}

void model::distribution::push_exp(string var, node *lambda)
{
  model::distribution::exp.insert(make_pair(var, lambda));
}

node *model::distribution::uniform_to_node(node *a, node *b)
{
  node *minus_node = new node("+", {b, a});
  return new node("/", {new node("1"), minus_node});
}

node *model::distribution::normal_to_node(string var, node *mu, node *sigma)
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

node *model::distribution::exp_to_node(string var, node *lambda)
{
  node *times_node = new node("*", {lambda, new node(var)});
  node *unary_minus_node = new node("-", {times_node});
  node *exp_node = new node("exp", {unary_minus_node});
  return new node("*", {exp_node, lambda});
}

void model::set_model_type()
{
  if (model::rv_map.empty() && model::dd_map.empty() && model::par_map.empty())
  {
    model::model_type = model::type::HA;
  }
  else if (model::par_map.empty())
  {
    model::model_type = model::type::PHA;
  }
  else
  {
    model::model_type = model::type::NPHA;
  }
}
