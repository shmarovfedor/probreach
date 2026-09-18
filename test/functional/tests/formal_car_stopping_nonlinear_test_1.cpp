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

TEST(formal_good_test_1, testing_good_1_pdrh)
{
  // parsing the model
  frontendt frontend;
  if (frontend.parse(
        string(PROBREACH_TEST_MODELS_DIR) + 
        string("/cars/car_stopping_nonlinear_1.pdrh")) != 0)
    return EXIT_FAILURE;
  
  std::cout << old::global_model.to_string() << "\n";
  // setting precision for computing the probability interval
  global_config.precision_prob = 5e-2;
  global_config.partition_prob = true;
  // computing the probability now
  capd::interval probability = formal::evaluate_pha(3, 3);
  std::cerr << "Probability interval: " << probability << "\n";
  EXPECT_LE(capd::intervals::width(probability), global_config.precision_prob);
  EXPECT_TRUE(capd::interval("0.01", "0.09").contains(probability));
}
