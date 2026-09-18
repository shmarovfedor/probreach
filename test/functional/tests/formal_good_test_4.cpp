#include <gtest/gtest.h>
#include <iostream>

#include "formal.h"
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

TEST(formal_good_test_4, testing_good_4_pdrh)
{
  // parsing the model
  frontendt frontend;
  if (frontend.parse(
        string(PROBREACH_TEST_MODELS_DIR) + string("/good/good_4.pdrh")) != 0)
    return EXIT_FAILURE;
  
  // setting precision for computing the probability interval
  global_config.partition_nondet = true;
  map<string, string> partition_nondet_map = {{"n", "0.1"}};
  global_config.partition_nondet_map = partition_nondet_map;
  global_config.partition_prob = true;
  map<string, string> partition_prob_map = {{"r", "0.01"}};
  global_config.partition_prob_map = partition_prob_map;
  // computing the probability now
  map<box, capd::interval> p_map = formal::evaluate_npha(0, 0);
  for (auto it = p_map.cbegin(); it != p_map.cend(); it++)
  {
    EXPECT_LE(capd::intervals::width(it->first.get_map()["n"]), 0.1);
    EXPECT_TRUE(it->second.contains(capd::interval("0.1", "0.1")));
    // these tests may not be successfull every time, and strictly
    // speaking, they do not have to. However, when they fail it might
    // serve as an indication that something might be wrong with the
    // implementation.
    EXPECT_LE(capd::intervals::width(it->second), 0.2);
    EXPECT_LT(it->second.rightBound(), 1);
    EXPECT_GT(it->second.leftBound(), 0);
  }
}
