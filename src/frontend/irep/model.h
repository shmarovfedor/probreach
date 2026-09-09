//
// Created by fedor on 24/01/16.
//

#ifndef PROBREACH_MODEL_H
#define PROBREACH_MODEL_H

#include <vector>
#include <map>
#include <tuple>
#include "node.h"

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

namespace distribution
{
extern std::map<std::string, std::pair<node *, node *>> uniform;
extern std::map<std::string, std::pair<node *, node *>> normal;
extern std::map<std::string, node *> exp;

void push_uniform(std::string, node *, node *);
void push_normal(std::string, node *, node *);
void push_exp(std::string, node *);

node *uniform_to_node(node *, node *);
node *normal_to_node(std::string, node *, node *);
node *exp_to_node(std::string, node *);
} // namespace distribution
// SYMBOL TABLE end

// mode struct
struct mode
{
  std::string id;
  std::vector<node *> invts;
  // jump struct
  struct jump
  {
    std::string next_id;
    node *guard;
    std::map<std::string, node *> reset;

    inline jump()
    {
    }

    inline jump(
      std::string next_id,
      node *guard,
      std::map<std::string, node *> reset)
      : next_id(next_id), guard(guard), reset(reset)
    {
    }
  };
  std::vector<jump> jumps;
  std::map<std::string, node *> odes;
  std::pair<node *, node *> time;
};
extern std::vector<mode> modes;

// state struct
struct state
{
  std::string id;
  node *prop;

  inline state(std::string id, node *prop) : id(id), prop(prop)
  {
  }

  inline state()
  {
  }

  friend std::ostream &operator<<(std::ostream &os, const model::state &st)
  {
    os << st.id << ":" << st.prop->to_prefix() << ";";
    return os;
  }
};

extern std::vector<state> init;
extern std::vector<state> goal;

// methods for updating the model
void push_var(std::string, node *, node *);
void push_dd(std::string, std::map<node *, node *>);
void push_rv(std::string, node *, node *, node *, node *);

void finalise();

// getter methods
mode *get_mode(std::string);
std::vector<mode *> get_successors(mode *);

// this actually generates paths of the given length (like symbolic execution);
// this should not be part of the irep
std::vector<std::vector<mode *>> get_paths(mode *, mode *, int);
std::vector<std::vector<mode *>> get_all_paths(int);
std::vector<std::vector<mode *>> get_all_paths(int, int);

std::string to_string();

} // namespace model

#endif //PROBREACH_MODEL_H
