//
// Created by fedor on 01/09/2026.
//

#ifndef PROBREACH_AST_H
#define PROBREACH_AST_H

#include <string>
#include <ostream>
#include <vector>
#include <memory>
#include <map>

#include "symbol.h"
#include "expr.h"
#include "dist.h"
#include "decl.h"
#include "interval.h"
#include "assign.h"

// Other model components
class invtt
{
private:
  std::unique_ptr<bool_exprt> cond;

public:
  invtt(std::unique_ptr<bool_exprt> cond) : cond(std::move(cond))
  {
  }

  std::string get_type() const
  {
    return "invtt";
  }

  void print(std::ostream &out) const
  {
    out << *cond;
  }

  friend std::ostream &operator<<(std::ostream &os, const invtt &e)
  {
    e.print(os);
    return os;
  }
  
  bool_exprt &get_condition()
  {
    return *cond;
  }
};

class odet
{
private:
  std::unique_ptr<symbolt> sym;
  std::unique_ptr<real_exprt> rhs;

public:
  odet(std::unique_ptr<symbolt> sym, std::unique_ptr<real_exprt> rhs)
    : sym(std::move(sym)), rhs(std::move(rhs))
  {
  }

  void print(std::ostream &out) const
  {
    out << "d/dt[" << *sym << "] = " << *rhs;
  }

  symbolt &get_symbol()
  {
    return *sym;
  }

  real_exprt &get_rhs()
  {
    return *rhs;
  }

  friend std::ostream &operator<<(std::ostream &os, const odet &e)
  {
    e.print(os);
    return os;
  }

  std::string get_type() const
  {
    return "odet";
  }
};

class flowt
{
private:
  std::vector<std::unique_ptr<odet>> odes;

public:
  flowt(std::vector<std::unique_ptr<odet>> odes) : odes(std::move(odes))
  {
  }

  std::string get_type() const
  {
    return "flowt";
  }

  std::vector<std::unique_ptr<odet>> &get_odes()
  {
    return odes;
  }

  void print(std::ostream &out) const
  {
    for (size_t i = 0; i < odes.size(); i++)
      out << *odes[i] << ";\n";
  }

  friend std::ostream &operator<<(std::ostream &os, const flowt &e)
  {
    e.print(os);
    return os;
  }
};

class statet
{
protected:
  std::unique_ptr<symbolt> mode_id;

public:
  statet(std::unique_ptr<symbolt> mode_id) : mode_id(std::move(mode_id))
  {
  }

  virtual ~statet() = default;

  symbolt &get_mode_id()
  {
    return *mode_id;
  }

  virtual void print(std::ostream &out) const = 0;
  virtual std::string get_type() const = 0;

  friend std::ostream &operator<<(std::ostream &os, const statet &e)
  {
    e.print(os);
    return os;
  }
};

class cond_statet : public statet
{
private:
  std::unique_ptr<bool_exprt> cond;

public:
  cond_statet(
    std::unique_ptr<symbolt> mode_id,
    std::unique_ptr<bool_exprt> cond)
    : statet(std::move(mode_id)), cond(std::move(cond))
  {
  }

  bool_exprt &get_condition()
  {
    return *cond;
  }

  void print(std::ostream &out) const override
  {
    out << "@" << *mode_id << " " << *cond;
  }

  std::string get_type() const override
  {
    return "cond_statet";
  }
};

class reset_statet : public statet
{
private:
  std::vector<std::unique_ptr<assignt>> assigns;

public:
  reset_statet(
    std::unique_ptr<symbolt> mode_id,
    std::vector<std::unique_ptr<assignt>> assigns)
    : statet(std::move(mode_id)), assigns(std::move(assigns))
  {
  }

  std::string get_type() const override
  {
    return "reset_statet";
  }

  std::vector<std::unique_ptr<assignt>> &get_assignments()
  {
    return assigns;
  }

  void print(std::ostream &out) const override
  {
    out << "@" << *mode_id << " (and ";
    for (size_t i = 0; i < assigns.size() - 1; i++)
      out << "(" << *assigns[i] << ") ";
    out << "(" << *assigns.back() << "))";
  }
};

class jumpt
{
private:
  std::unique_ptr<bool_exprt> guard;
  std::unique_ptr<reset_statet> reset;

public:
  jumpt(std::unique_ptr<bool_exprt> guard, std::unique_ptr<reset_statet> reset)
    : guard(std::move(guard)), reset(std::move(reset))
  {
  }

  std::string get_type() const
  {
    return "jumpt";
  }

  bool_exprt &get_guard()
  {
    return *guard;
  }

  reset_statet &get_reset()
  {
    return *reset;
  }

  void print(std::ostream &out) const
  {
    out << *guard << " ==> " << *reset;
  }

  friend std::ostream &operator<<(std::ostream &os, const jumpt &e)
  {
    e.print(os);
    return os;
  }
};

class modet
{
private:
  std::unique_ptr<symbolt> mode_id;
  std::unique_ptr<intervalt> time_domain;
  std::vector<std::unique_ptr<invtt>> invariants;
  std::unique_ptr<flowt> flow;
  // the jumps are stored as a map <successor_mode_id, jumpt>
  // instead of just a vector of <jumpt> so that it's easier to access
  // the ids of successor modes
  std::map<std::unique_ptr<symbolt>, std::unique_ptr<jumpt>> jumps;

public:
  modet(
    std::unique_ptr<symbolt> mode_id,
    std::unique_ptr<intervalt> time_domain,
    std::vector<std::unique_ptr<invtt>> invariants,
    std::unique_ptr<flowt> flow,
    std::map<std::unique_ptr<symbolt>, std::unique_ptr<jumpt>> jumps)
    : mode_id(std::move(mode_id)),
      time_domain(std::move(time_domain)),
      invariants(std::move(invariants)),
      flow(std::move(flow)),
      jumps(std::move(jumps))
  {
  }

  symbolt &get_mode_id()
  {
    return *mode_id;
  }

  intervalt &get_time_domain()
  {
    return *time_domain;
  }

  std::vector<std::unique_ptr<invtt>> &get_invariants()
  {
    return invariants;
  }

  flowt &get_flow()
  {
    return *flow;
  }
  
  std::map<std::unique_ptr<symbolt>, std::unique_ptr<jumpt>> &get_jumps()
  {
    return jumps;
  }

  std::string get_type() const
  {
    return "modet";
  }

  void print(std::ostream &out) const
  {
    out << "{\n";
    out << "mode " << *mode_id << ";\n";
    out << "time: " << *time_domain << ";\n";
    if (invariants.size() > 0)
    {
      out << "invt:\n";
      for (size_t i = 0; i < invariants.size(); i++)
        out << *invariants[i] << ";\n";
    }
    out << "flow:\n";
    out << *flow;
    if (!jumps.empty())
    {
      out << "jump:\n";
      for (auto it = jumps.cbegin(); it != jumps.cend(); ++it)
        out << *(it->second) << ";\n";
    }
    out << "}\n";
  }

  friend std::ostream &operator<<(std::ostream &os, const modet &e)
  {
    e.print(os);
    return os;
  }
};

class modelt
{
private:
  std::map<std::unique_ptr<symbolt>, std::unique_ptr<declt>> decls;
  std::map<std::unique_ptr<symbolt>, std::unique_ptr<modet>> modes;
  std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>> inits;
  std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>> goals;

public:
  modelt(
    std::map<std::unique_ptr<symbolt>, std::unique_ptr<declt>> decls,
    std::map<std::unique_ptr<symbolt>, std::unique_ptr<modet>> modes,
    std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>> inits,
    std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>> goals) :
  decls(std::move(decls)),
  modes(std::move(modes)),
  inits(std::move(inits)),
  goals(std::move(goals))
  {
  }

  std::string get_type() const
  {
    return "modelt";
  }

  std::map<std::unique_ptr<symbolt>, std::unique_ptr<declt>> &get_declarations()
  {
    return decls;
  }

  std::map<std::unique_ptr<symbolt>, std::unique_ptr<modet>> &get_modes()
  {
    return modes;
  }

  std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>> &get_inits()
  {
    return inits;
  }

  std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>> &get_goals()
  {
    return goals;
  }

  void print(std::ostream &out) const
  {
    for (auto it = decls.cbegin(); it != decls.cend(); ++it)
      out << *(it->second) << ";\n";

    for (auto it = modes.cbegin(); it != modes.cend(); ++it)
      out << *(it->second);

    out << "init:\n";
    for (auto it = inits.cbegin(); it != inits.cend(); ++it)
      out << *(it->second) << ";\n";
    
    out << "goal:\n";
    for (auto it = goals.cbegin(); it != goals.cend(); ++it)
      out << *(it->second) << ";\n";
  }

  friend std::ostream &operator<<(std::ostream &os, const modelt &e)
  {
    e.print(os);
    return os;
  }
};

#endif // PROBREACH_AST_H
