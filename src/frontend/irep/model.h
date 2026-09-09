//
// Created by fedor on 24/01/16.
//

#ifndef PROBREACH_MODEL_H
#define PROBREACH_MODEL_H

#include <vector>
#include <map>
#include <tuple>
#include "node.h"

class jumpt
{
public:
  std::string next_id;
  node *guard;
  std::map<std::string, node *> reset;

  jumpt()
  {
  }

  jumpt(
    std::string next_id,
    node *guard,
    std::map<std::string, node *> reset)
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

class declarationst
{
public:
  std::map<std::string, std::pair<node *, node *>> uniform;
  std::map<std::string, std::pair<node *, node *>> normal;
  std::map<std::string, node *> exp;
  
  declarationst()
  {
  }
};

namespace model
{
// model type
enum type
{
  HA,
  PHA,
  NHA,
  NPHA
};
extern type model_type;
void set_model_type();

// SYMBOL TABLE begin
extern std::map<std::string, std::tuple<node *, node *, node *, node *>> rv_map;
extern std::map<std::string, std::map<node *, node *>> dd_map;
extern std::map<std::string, std::pair<node *, node *>> var_map;
extern std::map<std::string, std::pair<node *, node *>> par_map;
extern declarationst declarations;
// SYMBOL TABLE end

extern std::vector<modet> modes;
extern std::vector<statet> init;
extern std::vector<statet> goal;

// methods for updating the model
void push_var(std::string, node *, node *);
void push_dd(std::string, std::map<node *, node *>);
void push_rv(std::string, node *, node *, node *, node *);
void push_uniform(std::string, node *, node *);
void push_normal(std::string, node *, node *);
void push_exp(std::string, node *);

node *uniform_to_node(node *, node *);
node *normal_to_node(std::string, node *, node *);
node *exp_to_node(std::string, node *);

void finalise();

// getter methods
modet *get_mode(std::string);
std::vector<modet *> get_successors(modet *);

// this actually generates paths of the given length (like symbolic execution);
// this should not be part of the irep
std::vector<std::vector<modet *>> get_paths(modet *, modet *, int);
std::vector<std::vector<modet *>> get_all_paths(int);
std::vector<std::vector<modet *>> get_all_paths(int, int);

std::string to_string();

} // namespace model

#endif //PROBREACH_MODEL_H
