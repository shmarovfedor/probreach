//
// Created by fedor on 21/10/26.
//
#ifndef PROBREACH_BUILD_TRANSITION_GRAPH_H
#define PROBREACH_BUILD_TRANSITION_GRAPH_H

#include "transition_graph.h"
#include "symbol_table.h"

class build_transition_grapht
{
private:
  const symbol_tablet &symbol_table;

private:
  const modet *resolve_target(jumpt &jump) const;
  const modet *lookup_mode(symbolt &symbol) const;

public:
  build_transition_grapht(const symbol_tablet &symbol_table)
    : symbol_table(symbol_table)
  {
  }

  void build(transition_grapht &graph) const;
};

#endif //PROBREACH_BUILD_TRANSITION_GRAPH_H
