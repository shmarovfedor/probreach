#include "new_to_old.h"
#include <iostream>

node *new_to_old(real_exprt &e)
{
  if (auto ptr = dynamic_cast<symbol_exprt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<numbert *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<minust *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<plust *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<addt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<subt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<mult *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<divt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<powt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<abst *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<sqrtt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<expt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<logt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<sint *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<cost *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<tant *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<asint *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<acost *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<atant *>(&e))
  {
    return new_to_old(*ptr);
  }
}

node *new_to_old(symbolt &e)
{
  return new node(e.get_value());
}

node *new_to_old(symbol_exprt &e)
{
  return new node(e.get_value());
}

node *new_to_old(numbert &e)
{
  return new node(e.get_value());
}

node *new_to_old(minust &e)
{
  return new node("-", {new_to_old(e.get_value())});
}

node *new_to_old(plust &e)
{
  return new node("+", {new_to_old(e.get_value())});
}

node *new_to_old(addt &e)
{
  return new node("+", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(subt &e)
{
  return new node("-", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(mult &e)
{
  return new node("*", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(divt &e)
{
  return new node("/", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(powt &e)
{
  return new node("^", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(abst &e)
{
  return new node("abs", {new_to_old(e.get_value())});
}

node *new_to_old(sqrtt &e)
{
  return new node("sqrt", {new_to_old(e.get_value())});
}

node *new_to_old(expt &e)
{
  return new node("exp", {new_to_old(e.get_value())});
}

node *new_to_old(logt &e)
{
  return new node("log", {new_to_old(e.get_value())});
}

node *new_to_old(sint &e)
{
  return new node("sin", {new_to_old(e.get_value())});
}

node *new_to_old(cost &e)
{
  return new node("cos", {new_to_old(e.get_value())});
}

node *new_to_old(tant &e)
{
  return new node("tan", {new_to_old(e.get_value())});
}

node *new_to_old(asint &e)
{
  return new node("asin", {new_to_old(e.get_value())});
}

node *new_to_old(acost &e)
{
  return new node("acos", {new_to_old(e.get_value())});
}

node *new_to_old(atant &e)
{
  return new node("atan", {new_to_old(e.get_value())});
}

node *new_to_old(bool_exprt &e)
{
  if (auto ptr = dynamic_cast<truet *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<falset *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<greater_thant *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<greater_equalt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<less_thant *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<less_equalt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<equalt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<not_equalt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<nott *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<implyt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<andt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<ort *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<xort *>(&e))
  {
    return new_to_old(*ptr);
  }
}

node *new_to_old(truet &e)
{
  return new node("(true)");
}

node *new_to_old(falset &e)
{
  return new node("(false)");
}

node *new_to_old(greater_thant &e)
{
  return new node(">", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(greater_equalt &e)
{
  return new node(">=", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(less_thant &e)
{
  return new node("<", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(less_equalt &e)
{
  return new node("<=", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(equalt &e)
{
  return new node("=", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(not_equalt &e)
{
  return new node("!=", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(nott &e)
{
  return new node("not", {new_to_old(e.get_value())});
}

node *new_to_old(implyt &e)
{
  return new node("=>", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(andt &e)
{
  auto old_node = new node("and");
  for (auto &it : e.get_operands())
    old_node->operands.push_back(new_to_old(*it));
  return old_node;
}

node *new_to_old(ort &e)
{
  auto old_node = new node("or");
  for (auto &it : e.get_operands())
    old_node->operands.push_back(new_to_old(*it));
  return old_node;
}

node *new_to_old(xort &e)
{
  auto old_node = new node("xor");
  for (auto &it : e.get_operands())
    old_node->operands.push_back(new_to_old(*it));
  return old_node;
}

std::pair<node *, node *> new_to_old(intervalt &e)
{
  std::pair<node *, node *> res;
  res.first = new_to_old(e.get_left());
  res.second = new_to_old(e.get_right());
  return res;
}

std::vector<node *> new_to_old(std::vector<std::unique_ptr<invtt>> &e)
{
  std::vector<node *> res;
  for (auto &it : e)
    res.push_back(new_to_old(it->get_condition()));
  return res;
}

std::pair<std::string, node *> new_to_old(assignt &e)
{
  std::pair<std::string, node *> res(
    e.get_symbol().get_value(), new_to_old(e.get_rhs()));
  return res;
}

node *new_to_old(rvaluet &e)
{
  if (auto ptr = dynamic_cast<real_exprt *>(&e))
  {
    return new_to_old(*ptr);
  }
}

std::map<std::string, node *>
new_to_old(std::vector<std::unique_ptr<assignt>> &e)
{
  std::map<std::string, node *> res;
  for (auto &it : e)
  {
    std::pair<std::string, node *> vals = new_to_old(*it);
    res.insert(vals);
  }
  return res;
}

std::pair<std::string, std::map<std::string, node *>>
new_to_old(reset_statet &e)
{
  std::pair<std::string, std::map<std::string, node *>> res(
    e.get_mode_id().get_value(), new_to_old(e.get_assignments()));
  return res;
}

old::statet new_to_old(cond_statet &e)
{
  old::statet res;
  res.id = e.get_mode_id().get_value();
  res.prop = new_to_old(e.get_condition());
  return res;
}

std::vector<old::statet>
new_to_old(std::map<std::unique_ptr<symbolt>, std::unique_ptr<cond_statet>> &e)
{
  std::vector<old::statet> res;
  for (auto &it : e)
    res.push_back(new_to_old(*(it.second)));
  return res;
}

std::map<std::string, node *> new_to_old(flowt &e)
{
  std::map<std::string, node *> res;
  for (auto &it : e.get_odes())
    res.emplace(it->get_symbol().get_value(), new_to_old(it->get_rhs()));
  return res;
}

old::jumpt new_to_old(jumpt &e)
{
  old::jumpt res;
  res.next_id = e.get_reset().get_mode_id().get_value();
  res.guard = new_to_old(e.get_guard());
  res.reset = new_to_old(e.get_reset()).second;
  return res;
}

std::vector<old::jumpt>
new_to_old(std::map<std::unique_ptr<symbolt>, std::unique_ptr<jumpt>> &e)
{
  std::vector<old::jumpt> res;
  for (auto &it : e)
    res.push_back(new_to_old(*(it.second)));
  return res;
}

old::modet new_to_old(modet &e)
{
  old::modet res;
  res.id = e.get_mode_id().get_value();
  res.invts = new_to_old(e.get_invariants());
  res.jumps = new_to_old(e.get_jumps());
  res.odes = new_to_old(e.get_flow());
  res.time = new_to_old(e.get_time_domain());
  return res;
}

std::vector<old::modet>
new_to_old(std::map<std::unique_ptr<symbolt>, std::unique_ptr<modet>> &e)
{
  std::vector<old::modet> res;
  for (auto &it : e)
    res.push_back(new_to_old(*(it.second)));
  return res;
}

node *new_to_old(discrete_distt &e)
{
  node *params = new node();
  for (auto &it : e.get_p_mass())
  {
    node *dd_pair =
      new node(":", {new_to_old(*(it.first)), new_to_old(*(it.second))});
    params->operands.push_back(dd_pair);
  }
  return new node("dist_discrete", {params});
}

node *new_to_old(normal_distt &e)
{
  return new node(
    "dist_normal", {new_to_old(e.get_mu()), new_to_old(e.get_sigma())});
}

node *new_to_old(uniform_distt &e)
{
  return new node(
    "dist_uniform", {new_to_old(e.get_left()), new_to_old(e.get_right())});
}

node *new_to_old(exp_distt &e)
{
  return new node("dist_exp", {new_to_old(e.get_lambda())});
}

node *new_to_old(distt &e)
{
  if (auto ptr = dynamic_cast<discrete_distt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<normal_distt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<uniform_distt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<exp_distt *>(&e))
  {
    return new_to_old(*ptr);
  }
}

old::declarationt *new_to_old(declt &e)
{
  if (auto ptr = dynamic_cast<const_declt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<var_declt *>(&e))
  {
    return new_to_old(*ptr);
  }
  else if (auto ptr = dynamic_cast<dist_declt *>(&e))
  {
    return new_to_old(*ptr);
  }
}

old::declarationt *new_to_old(const_declt &e)
{
  node *decl = new node("const_decl", {new_to_old(e.get_value())});
  return new old::declarationt(e.get_symbol().get_value(), decl);
}

old::declarationt *new_to_old(var_declt &e)
{
  node *decl = new node(
    "var_decl",
    {new_to_old(e.get_domain().get_left()),
     new_to_old(e.get_domain().get_right())});
  return new old::declarationt(e.get_symbol().get_value(), decl);
}

old::declarationt *new_to_old(dist_declt &e)
{
  return new old::declarationt(
    e.get_symbol().get_value(),
    new node("dist_decl", {new_to_old(e.get_dist())}));
}

std::map<std::string, old::declarationt>
new_to_old(std::map<std::unique_ptr<symbolt>, std::unique_ptr<declt>> &e)
{
  std::map<std::string, old::declarationt> res;
  for (auto &it : e)
    res.emplace(it.first->get_value(), *new_to_old(*(it.second)));
  return res;
}

old::modelt new_to_old(modelt &e)
{
  old::modelt res;
  res.declarations.decls = new_to_old(e.get_symbol_table());
  res.modes = new_to_old(e.get_modes());
  res.init = new_to_old(e.get_inits());
  res.goal = new_to_old(e.get_goals());
  return res;
}
