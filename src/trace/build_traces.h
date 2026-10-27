//
// Created by fedor on 23/10/26.
//
#ifndef PROBREACH_BUILD_TRACES_H
#define PROBREACH_BUILD_TRACES_H

#include <vector>

#include "ast.h"
#include "transition_graph.h"
#include "trace.h"
#include "symbol_table.h"

class build_tracest
{
private:
  modelt &model;
  const transition_grapht &graph;
  const symbol_tablet &sym_table;
  const size_t depth;

  tracet current_trace;
  size_t init_idx = 0;
  size_t goal_idx = 0;

  struct framet
  {
    const modet* current_mode;
    size_t next_successor;
  };

  std::vector<framet> stack;
  bool trace_pending = false;

public:
  build_tracest(
      modelt &model,
      const transition_grapht &graph,
      const symbol_tablet &sym_table,
      const size_t depth)
    : model(model), graph(graph), sym_table(sym_table), depth(depth)
  {
    
  }

  tracet *next();

private:
  bool initialise_next_init_goal_pair();

};

#endif //PROBREACH_BUILD_TRACES_H
