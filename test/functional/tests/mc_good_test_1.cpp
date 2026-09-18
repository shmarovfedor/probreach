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

TEST(mc_good_test_1, testing_good_1_pdrh)
{
  // parsing the model
  frontendt frontend;
  if (frontend.parse(
        string(PROBREACH_TEST_MODELS_DIR) + string("/good/good_1.pdrh")) != 0)
    return EXIT_FAILURE;
  
  // setting precision for computing the probability interval
  double acc = 2.5e-2;
  double conf = 0.99;
  // computing the probability now
  capd::interval probability =
    algorithm::evaluate_pha_bayesian(0, 0, acc, conf, {});
  // true probability in this example is 0.1; so the probability interval:
  // 1) must contain 0.1
  // 2) its width must be <= "global_config.precision_prob"
  std::cerr << "Probability interval: " << probability << "\n";
  EXPECT_TRUE(probability.contains(capd::interval("0.1", "0.1")));
  EXPECT_NEAR(capd::intervals::width(probability), 2 * acc, 1e-6);
}
