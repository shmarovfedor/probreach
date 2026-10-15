//
// Created by fedor on 24/01/16.
//

#include <sstream>
#include <algorithm>
#include <set>
#include <iostream>

#include "model.h"

using namespace std;

old::modelt::type old::modelt::get_model_type()
{
  if (
    sym_table.rv_map.empty() && sym_table.dd_map.empty() &&
    sym_table.par_map.empty())
  {
    return type::HA;
  }
  else if (sym_table.par_map.empty())
  {
    return type::PHA;
  }
  else
  {
    return type::NPHA;
  }
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
  out << "MODEL TYPE: " << get_model_type() << endl;
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
