//
// Created by fedor on 12/06/18.
//

#ifndef PROBREACH_NODE_UTILS_H
#define PROBREACH_NODE_UTILS_H

#include <capd/capdlib.h>
#include <capd/intervals/lib.h>
#include <vector>

#include "box.h"
#include "node.h"

class node_utils
{
public:
  
  static capd::interval node_to_interval(node *);
  static capd::interval node_to_interval(node *, std::vector<box>);

  static double node_to_double(node *);
  static double node_to_double(node *, std::map<std::string, double>);
  static bool node_to_boolean(node *, std::map<std::string, double>);
};

#endif // PROBREACH_NODE_UTILS_H
