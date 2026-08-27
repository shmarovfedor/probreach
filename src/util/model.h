//
// Created by fedor on 24/01/16.
//

#ifndef PROBREACH_MODEL_H
#define PROBREACH_MODEL_H

#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include "node.h"

namespace pdrh
{
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
extern std::pair<node *, node *> time;
extern std::map<std::string, std::tuple<node *, node *, node *, node *>> rv_map;
extern std::map<std::string, std::string> rv_type_map;
extern std::map<std::string, std::map<node *, node *>> dd_map;
extern std::map<std::string, std::pair<node *, node *>> var_map;
extern std::map<std::string, std::pair<node *, node *>> par_map;
extern std::map<std::string, std::string> const_map;

namespace distribution
{
extern std::map<std::string, std::pair<node *, node *>> uniform;
extern std::map<std::string, std::pair<node *, node *>> normal;
extern std::map<std::string, node *> exp;
extern std::map<std::string, std::pair<node *, node *>> gamma;

void push_uniform(std::string, node *, node *);
void push_normal(std::string, node *, node *);
void push_exp(std::string, node *);
void push_gamma(std::string, node *, node *);

node *uniform_to_node(node *, node *);
node *normal_to_node(std::string, node *, node *);
node *exp_to_node(std::string, node *);
node *gamma_to_node(std::string, node *, node *);
} // namespace distribution
// SYMBOL TABLE end
bool var_exists(std::string);

// mode struct
struct mode
{
  int id;
  std::vector<node *> invts;
  // jump struct
  struct jump
  {
    int next_id;
    node *guard;
    std::map<std::string, node *> reset;
    std::map<
      std::string,
      std::tuple<
        std::string,
        node *,
        node *,
        node *,
        node *>>
      reset_rv;
    std::map<std::string, std::map<node *, node *>> reset_dd;
    std::map<std::string, std::pair<node *, node *>> reset_nondet;

    inline jump()
    {
    }

    inline jump(
      const int next_id,
      node *guard,
      std::map<std::string, node *> reset)
      : next_id(next_id), guard(guard), reset(reset)
    {
    }
  };
  std::vector<jump> jumps;
  std::map<std::string, std::pair<node *, node *>> flow_map;
  std::map<std::string, node *> odes;
  std::pair<node *, node *> time;

  pdrh::mode::jump get_jump(int);
};
extern std::vector<mode> modes;

// state struct
struct state
{
  int id;
  node *prop;

  inline state(const int id, node *prop) : id(id), prop(prop)
  {
  }

  inline state()
  {
  }

  friend std::ostream &operator<<(std::ostream &os, const pdrh::state &st)
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
void push_rv_type(std::string, std::string);
void push_mode(mode);
void push_invt(mode &, node *);
// first argument is the address of the mode, second argument is the variable name, third argument is the ode
void push_ode(mode &, std::string, node *);
void push_jump(mode &, mode::jump);
// first argument is the address of the jump, second argument is the variable name, third argument is the reset expression
void push_reset(mode &, mode::jump &, std::string, node *);
void push_init(std::vector<state>);
void push_goal(std::vector<state>);
void push_time_bounds(node *, node *);

void output_traj(std::vector<std::map<std::string, double>>, std::ostream &);

mode *get_mode(int);
std::vector<mode *> get_init_modes();
std::vector<mode *> get_goal_modes();
std::vector<mode *> get_successors(mode *);
std::vector<std::vector<mode *>> get_paths(mode *, mode *, int);
std::vector<std::vector<mode *>> get_all_paths(int);
std::vector<std::vector<mode *>> get_all_paths(int, int);

// returns <first_map_keys> \ <second_map_keys>
std::vector<std::string> get_keys_diff(
  std::map<std::string, std::pair<node *, node *>>,
  std::map<std::string, std::pair<node *, node *>>);

std::string model_to_string();

} // namespace pdrh

#endif //PROBREACH_MODEL_H
