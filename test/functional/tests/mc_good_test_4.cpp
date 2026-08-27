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

TEST(mc_good_test_4, testing_good_4_pdrh)
{
  parse_pdrh_model(
    string(PROBREACH_TEST_MODELS_DIR) + string("/good/good_4.pdrh"));
  // setting precision for computing the probability interval
  double acc = 2.5e-2;
  double conf = 0.99;
  int size = 5;
  int iter_num = 3;

  pair<box, capd::interval> res = algorithm::evaluate_npha_cross_entropy_normal(
    0, 0, size, iter_num, acc, conf);
  std::cerr << "Nondet parameter value: " << res.first << "\n";
  std::cerr << "Probability interval: " << res.second << "\n";
  EXPECT_TRUE(res.second.contains(capd::interval("0.1", "0.1")));
  EXPECT_NEAR(capd::intervals::width(res.second), 2 * acc, 1e-6);
}
