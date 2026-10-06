//
// Created by fedor on 24/01/16.
//

#ifndef PROBREACH_MODEL_H
#define PROBREACH_MODEL_H

#include <vector>
#include <map>
#include <tuple>

#include "node.h"
#include "symbol_table.h"

namespace old
{

class jumpt
{
public:
  std::string next_id;
  node *guard;
  std::map<std::string, node *> reset;

  jumpt()
  {
  }

  jumpt(std::string next_id, node *guard, std::map<std::string, node *> reset)
    : next_id(next_id), guard(guard), reset(reset)
  {
  }
};

class statet
{
public:
  std::string id;
  node *prop;

  statet(std::string id, node *prop) : id(id), prop(prop)
  {
  }

  statet()
  {
  }
};

class modet
{
public:
  std::string id;
  std::vector<node *> invts;
  std::vector<jumpt> jumps;
  std::map<std::string, node *> odes;
  std::pair<node *, node *> time;

  modet()
  {
  }
};

class declarationt
{
public:
  std::string sym;
  node *decl;

  declarationt(std::string sym, node *decl) : sym(sym), decl(decl)
  {
  }
};

class modelt
{
public:
  // model type
  enum type
  {
    HA,
    PHA,
    NPHA
  };

  type model_type;
  symbol_tablet sym_table;
  std::map<std::string, declarationt> decls;
  std::vector<modet> modes;
  std::vector<statet> init;
  std::vector<statet> goal;

  modelt()
  {
  }

  modelt(
    std::map<std::string, declarationt> decls,
    std::vector<modet> modes,
    std::vector<statet> init,
    std::vector<statet> goal)
    : decls(decls), modes(modes), init(init), goal(goal)
  {
  }

  // getter methods
  modet *get_mode(std::string);
  std::string to_string();

  // methods for updating the model (should be part of typechecker/visitor)
  void build_symbol_table();
  void build_nondet_parameter_map();
  void complete_flows();
  void complete_resets();
  void set_model_type();
  void finalise();
};

extern modelt global_model;

}

#endif //PROBREACH_MODEL_H
