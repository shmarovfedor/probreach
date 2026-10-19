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
  std::cout << model << "\n";
}
