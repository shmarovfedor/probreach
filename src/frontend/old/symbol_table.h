//
// Created by fedor on 24/01/16.
//

#ifndef PROBREACH_SYMBOL_TABLE_H
#define PROBREACH_SYMBOL_TABLE_H

#include <vector>
#include <map>
#include <tuple>

#include "node.h"

namespace old
{

class symbol_tablet
{
public:
  /// distributions info
  std::map<std::string, std::pair<node *, node *>> uniform;
  std::map<std::string, std::pair<node *, node *>> normal;
  std::map<std::string, node *> exp;
  std::map<std::string, std::map<node *, node *>> dd_map;
  /// bounds info
  std::map<std::string, std::tuple<node *, node *, node *, node *>> rv_map;
  std::map<std::string, std::pair<node *, node *>> var_map;
  std::map<std::string, std::pair<node *, node *>> par_map;

  symbol_tablet()
  {
  }
  
  void push_var(std::string, node *, node *);
  void push_dd(std::string, std::map<node *, node *>);
  void push_rv(std::string, node *, node *, node *, node *);
  void push_uniform(std::string, node *, node *);
  void push_normal(std::string, node *, node *);
  void push_exp(std::string, node *);

  node *uniform_to_node(node *, node *);
  node *normal_to_node(std::string, node *, node *);
  node *exp_to_node(std::string, node *);
};

}

#endif //PROBREACH_SYMBOL_TABLE_H
