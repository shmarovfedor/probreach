//
// Created by fedor on 12/06/18.
//

#ifndef PROBREACH_PDRH2BOX_H
#define PROBREACH_PDRH2BOX_H

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
} // namespace pdrh
#endif // PROBREACH_PDRH2BOX_H
