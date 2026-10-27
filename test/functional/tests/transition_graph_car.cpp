#include <gtest/gtest.h>
#include <iostream>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "test_env.h"
#include "frontend.h"
#include "ast.h"
#include "symbol_table.h"
#include "build_symbol_table.h"
#include "complete_model.h"
#include "transition_graph.h"
#include "build_transition_graph.h"
#include "trace.h"
#include "build_traces.h"

using namespace std;

TEST(transition_graph_car_test, testing_transition_graph_car)
{
  // parsing the model
  frontendt frontend;
  if (frontend.parse(
        string(PROBREACH_TEST_MODELS_DIR) + 
        string("/cars/car_stopping_nonlinear_1.pdrh")) != 0)
    return EXIT_FAILURE;

  modelt &model = frontend.get_model(); 
  symbol_tablet sym_table;
  build_symbol_tablet builder;
  builder.build(sym_table, &model);
  complete_modelt completer;
  completer.run(sym_table, model);
  build_transition_grapht transition_graph_builder(sym_table);
  transition_grapht graph;
  transition_graph_builder.build(graph);
  std::cout << model << "\n";
  std::cout << "Transition graph:\n";
  std::cout << graph;
  build_tracest trace_gen(model, graph, sym_table, 3);
  std::cout << "Traces:\n";
  while (tracet* trace = trace_gen.next())
  {
    std::cout << *trace << "\n";
    std::cout << "----------\n";
  }
}
