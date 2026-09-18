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

#include "frontend.h"

using namespace std;

TEST(mc_good_test_4, testing_good_4_pdrh)
{
  // parsing the model
  frontendt frontend;
  if (frontend.parse(
        string(PROBREACH_TEST_MODELS_DIR) + string("/good/good_4.pdrh")) != 0)
    return EXIT_FAILURE;
  
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
