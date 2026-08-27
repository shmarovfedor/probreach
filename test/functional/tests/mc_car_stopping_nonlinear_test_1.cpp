#include <gtest/gtest.h>
#include <iostream>

#include "mc.h"
#include "pdrh_config.h"
#include "node.h"
#include "model.h"
#include "git_sha1.h"
#include "version.h"
#include "box.h"
#include "node_utils.h"
#include "decision_procedure.h"
#include "solver/dreal_wrapper.h"
#include "test_env.h"

#ifdef _OPENMP
#include <omp.h>
#endif

extern "C"
{
#include "pdrhparser.h"
}

extern "C" int yyparse();
extern "C" FILE *yyin;

using namespace std;

static void parse_pdrh_model(string filepath)
{
  // opening the model file
  FILE *pdrhfile = fopen(filepath.c_str(), "r");
  if (!pdrhfile)
  {
    cerr << "Couldn't open the file: " << filepath << endl;
    exit(EXIT_FAILURE);
  }
  // set lex to read from it instead of defaulting to STDIN:
  yyin = pdrhfile;
  // parse through the input until there is no more:
  do
  {
    yyparse();
  } while (!feof(yyin));
}

TEST(mc_cars_stopping_nonliner_test_1, testing_cars_stopping_1_pdrh)
{
  parse_pdrh_model(
    string(PROBREACH_TEST_MODELS_DIR) +
    string("/cars/car_stopping_nonlinear_1.pdrh"));
  // setting precision for computing the probability interval
  double acc = 2e-2;
  double conf = 0.99;
  // computing the probability now
  capd::interval probability =
    algorithm::evaluate_pha_bayesian(3, 3, acc, conf, {});
  std::cerr << "Probability interval: " << probability << "\n";
  EXPECT_TRUE(capd::interval("0.01", "0.09").contains(probability));
  EXPECT_NEAR(capd::intervals::width(probability), 2 * acc, 1e-6);
}
