//
// Created by fedor on 12/06/18.
//

#ifndef PROBREACH_NODE_UTILS_H
#define PROBREACH_NODE_UTILS_H

#include <capd/capdlib.h>
#include <capd/intervals/lib.h>

#include <iostream>
#include <tuple>
#include <vector>

#include "box.h"
#include "node.h"

namespace pdrh
{
capd::interval node_to_interval(node *);
capd::interval node_to_interval(node *, std::vector<box>);
bool node_to_boolean(node *, std::vector<box>);
node *box_to_node(box);
bool check_zero_crossing(node *, std::vector<box>, box, box);
box get_nondet_domain();
box get_domain();
box init_to_box(std::vector<box>);
double node_to_double(node *);
double node_to_double(node *, std::map<std::string, double>);
bool node_to_boolean(node *, std::map<std::string, double>);
bool node_zero_crossing(
  node *,
  std::map<std::string, double>,
  std::map<std::string, double>);

// used only in a handful of cases
node *copy_node(node *);
void copy_tree(node *&, node *);
void delete_node(node *);

// we might not need these methods in future
void get_first_node_by_value(node *, node *, std::vector<std::string>);
node *get_node_neg_by_value(node *, std::vector<std::string>);
} // namespace pdrh
#endif // PROBREACH_NODE_UTILS_H
