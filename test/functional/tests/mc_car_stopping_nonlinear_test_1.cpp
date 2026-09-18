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

TEST(mc_cars_stopping_nonliner_test_1, testing_cars_stopping_1_pdrh)
{
  // parsing the model
  frontendt frontend;
  if (frontend.parse(
      string(PROBREACH_TEST_MODELS_DIR) + 
      string("/cars/car_stopping_nonlinear_1.pdrh")) != 0)
    return EXIT_FAILURE;
  
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
