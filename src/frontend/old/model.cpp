//
// Created by fedor on 24/01/16.
//

#include <sstream>
#include <algorithm>
#include <set>
#include <iostream>

#include "model.h"

using namespace std;

void old::modelt::build_symbol_table()
{
  // extracting sym_table and populating the maps
  for (auto it = decls.cbegin(); it != decls.cend(); ++it)
  {
    declarationt decl = it->second;
    if (decl.decl->value == "const_decl")
    {
      sym_table.push_var(decl.sym, decl.decl->operands[0], decl.decl->operands[0]);
    }
    else if (decl.decl->value == "var_decl")
    {
      sym_table.push_var(decl.sym, decl.decl->operands[0], decl.decl->operands[1]);
    }
    else if (decl.decl->value == "dist_decl")
    {
      node *decl_node = decl.decl->operands[0];
      if (decl_node->value == "dist_normal")
      {
        sym_table.push_normal(
          decl.sym, decl_node->operands[0], decl_node->operands[1]);
      }
      else if (decl_node->value == "dist_uniform")
      {
        sym_table.push_uniform(
          decl.sym, decl_node->operands[0], decl_node->operands[1]);
      }
      else if (decl_node->value == "dist_exp")
      {
        sym_table.push_exp(
          decl.sym, decl_node->operands[0]);
      }
      else if (decl_node->value == "dist_discrete")
      {
        std::map<node*, node*> dd_pairs;
        for (node* op : decl_node->operands)
          dd_pairs.insert(make_pair(op->operands[0], op->operands[1]));
        
        sym_table.push_dd(decl.sym, dd_pairs);
      }
    }
  }
}

void old::modelt::build_nondet_parameter_map()
{
  // collecting all variables for which an ode is defined
  std::set<std::string> flow_vars;
  for (old::modet m : modes)
    for (auto it : m.odes)
      flow_vars.insert(it.first);

  // creating par_map (i.e. explicit nondet parameters)
  for (auto it : sym_table.var_map)
  {
    if (
      flow_vars.find(it.first) == flow_vars.cend() &&
      sym_table.par_map.find(it.first) ==
        sym_table.par_map.cend() &&
      sym_table.rv_map.find(it.first) ==
        sym_table.rv_map.cend() &&
      sym_table.dd_map.find(it.first) ==
        sym_table.dd_map.cend())
    {
      sym_table.par_map.insert(make_pair(it.first, it.second));
    }
  }
}

void old::modelt::complete_flows()
{
  // adding equations for the parameters (nondet and random)
  for (size_t i = 0; i < modes.size(); ++i)
  {
    for (auto it : sym_table.par_map)
      modes[i].odes.insert(make_pair(it.first, new node("0")));
    for (auto it : sym_table.rv_map)
      modes[i].odes.insert(make_pair(it.first, new node("0")));
    for (auto it : sym_table.dd_map)
      modes[i].odes.insert(make_pair(it.first, new node("0")));
  }
}

void old::modelt::complete_resets()
{
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
}

void old::modelt::set_model_type()
{
  if (
    sym_table.rv_map.empty() && sym_table.dd_map.empty() &&
    sym_table.par_map.empty())
  {
    old::modelt::model_type = type::HA;
  }
  else if (sym_table.par_map.empty())
  {
    old::modelt::model_type = type::PHA;
  }
  else
  {
    old::modelt::model_type = type::NPHA;
  }
}

void old::modelt::finalise()
{
  // the order of operations is important
  build_symbol_table();
  build_nondet_parameter_map();
  // the order is not important after this point
  complete_flows();
  complete_resets();
  set_model_type();
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

// getting string representation of the model
string old::modelt::to_string()
{
  stringstream out;
  out << "MODEL TYPE: " << model_type << endl;
  out << "DECLARATIONS:" << endl;
  for (auto it = decls.cbegin(); it != decls.cend(); ++it)
  {
    out << "|   " << it->first << " : " << *(it->second.decl) << "\n";
  }
  out << "VARIABLES:" << endl;
  for (auto it = sym_table.var_map.cbegin();
       it != sym_table.var_map.cend();
       ++it)
  {
    out << "|   " << it->first << " [" << it->second.first->to_prefix() << ", "
        << it->second.second->to_prefix() << "]" << endl;
  }
  out << "PARAMETERS:" << endl;
  for (auto it = sym_table.par_map.cbegin();
       it != sym_table.par_map.cend();
       it++)
  {
    out << "|   " << it->first << " [" << it->second.first->to_prefix() << ", "
        << it->second.second->to_prefix() << "]" << endl;
  }
  out << "CONTINUOUS RANDOM VARIABLES:" << endl;
  for (auto it = sym_table.rv_map.cbegin();
       it != sym_table.rv_map.cend();
       it++)
  {
    out << "|   pdf(" << it->first << ") = " << *(get<0>(it->second)) << "  | "
        << get<1>(it->second)->to_prefix() << " |   "
        << get<2>(it->second)->to_prefix() << "    |   "
        << get<3>(it->second)->to_prefix() << endl;
  }
  out << "DISCRETE RANDOM VARIABLES:" << endl;
  for (auto it = sym_table.dd_map.cbegin();
       it != sym_table.dd_map.cend();
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
