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

namespace pdrh
{
capd::interval node_to_interval(node *);
capd::interval node_to_interval(node *, std::vector<box>);

double node_to_double(node *);
double node_to_double(node *, std::map<std::string, double>);
bool node_to_boolean(node *, std::map<std::string, double>);
} // namespace pdrh
#endif // PROBREACH_NODE_UTILS_H
