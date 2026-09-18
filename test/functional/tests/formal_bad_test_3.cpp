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

TEST(formal_bad_test_3, testing_bad_3_pdrh)
{
  // parsing the model
  frontendt frontend;
  if (frontend.parse(
        string(PROBREACH_TEST_MODELS_DIR) + string("/bad/bad_3.pdrh")) != 0)
    return EXIT_FAILURE;
  
  // setting precision for computing the probability interval
  global_config.precision_prob = 1e-3;
  // computing the probability now
  capd::interval probability = formal::evaluate_pha(0, 0);
  // true probability in this example lies in the [0.625, 0.672] interval;
  // so the probability interval:
  // 1) must be within [0.625, 0.672]
  // 2) its width must be <= "global_config.precision_prob"
  EXPECT_TRUE(capd::interval("0.625", "0.672").contains(probability));
  EXPECT_LE(capd::intervals::width(probability), global_config.precision_prob);
}
