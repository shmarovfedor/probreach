//
// Created by fedor on 03/03/16.
//

#include <gsl/gsl_rng.h>
#include <gsl/gsl_qrng.h>
#include <gsl/gsl_cdf.h>
#include <capd/intervals/lib.h>
#include "mc.h"
#include "pdrh_config.h"
#include "measure.h"
#include "box_factory.h"
#include <chrono>
#include <iomanip>
#include <omp.h>
#include "rnd.h"
#include "node_utils.h"
#include "naive.h"
#include "solver/dreal_wrapper.h"

using namespace std;

capd::interval algorithm::evaluate_pha_chernoff(
  int min_depth,
  int max_depth,
  double acc,
  double conf,
  vector<box> nondet_boxes)
{
  const gsl_rng_type *T;
  gsl_rng *r;
  gsl_rng_env_setup();
  T = gsl_rng_default;
  // creating random generator
  r = gsl_rng_alloc(T);
  // setting the seed
  gsl_rng_set(
    r,
    std::chrono::system_clock::now().time_since_epoch() /
      std::chrono::milliseconds(1));
  // getting sample size using the Chernoff bound formula
  long int sample_size =
    (long int)std::ceil((1 / (2 * acc * acc)) * std::log(2 / (1 - conf)));
  //    long int sample_size = algorithm::get_cernoff_bound(acc, std::sqrt(conf));
  long int sat = 0;
  long int unsat = 0;
  if (global_config.verbose)
    cout << setprecision(16);
  if (global_config.verbose_result)
    cout << "Chernoff-Hoeffding algorithm started\n";
  if (global_config.verbose_result)
    cout << "Random sample size: " << sample_size << "\n";
#pragma omp parallel for schedule(dynamic)
  for (long int ctr = 0; ctr < sample_size; ctr++)
  {
    // getting all paths
    std::vector<std::vector<pdrh::mode *>> paths =
      pdrh::get_all_paths(min_depth, max_depth);
    // getting a sample
    box b = rnd::get_random_sample(r);
    if (global_config.verbose)
      cout << "Random sample: " << b << "\n";
    std::vector<box> boxes = {b};
    boxes.insert(boxes.end(), nondet_boxes.begin(), nondet_boxes.end());
    int undet_counter = 0;
    bool sat_flag = false;
    // evaluating all paths
    for (std::vector<pdrh::mode *> path : paths)
    {
      std::stringstream p_stream;
      for (pdrh::mode *m : path)
      {
        p_stream << m->id << " ";
      }
      // removing trailing whitespace
      if (global_config.verbose)
        cout << "Path: "
             << p_stream.str().substr(0, p_stream.str().find_last_of(" "))
             << "\n";
      int res;
      if (global_config.delta_sat)
      {
        res = decision_procedure::evaluate_delta_sat(
          path, boxes, global_config.solver_bin, global_config.solver_opt);
      }
      else
      {
        res = decision_procedure::evaluate(
          path, boxes, global_config.solver_bin, global_config.solver_opt);
      }
#pragma omp critical
      {
        if (res == decision_procedure::SAT)
        {
          if (global_config.verbose)
            cout << "SAT\n";
          sat++;
          sat_flag = true;
        }
        else if (res == decision_procedure::UNSAT)
        {
          if (global_config.verbose)
            cout << "UNSAT\n";
        }
        else if (res == decision_procedure::UNDET)
        {
          if (global_config.verbose)
            cout << "UNDET\n";
          undet_counter++;
        }
      }
      if (sat_flag)
      {
        break;
      }
    }
    // updating unsat counter
#pragma omp critical
    {
      if ((undet_counter == 0) && (!sat_flag))
      {
        unsat++;
      }
      if (global_config.verbose)
        cout << "CI: "
             << capd::interval(
                  ((double)sat / (double)sample_size) - acc,
                  ((double)(sample_size - unsat) / (double)sample_size) + acc)
             << "\n";
      if (global_config.verbose)
        cout << "Progress: " << (double)ctr / (double)sample_size << "\n";
    }
  }
  gsl_rng_free(r);
  if (global_config.verbose_result)
    cout << "Chernoff-Hoeffding algorithm finished\n";
  return capd::interval(
    ((double)sat / (double)sample_size) - acc,
    ((double)(sample_size - unsat) / (double)sample_size) + acc);
}

capd::interval algorithm::evaluate_pha_bayesian(
  int min_depth,
  int max_depth,
  double acc,
  double conf,
  vector<box> nondet_boxes)
{
  const gsl_rng_type *T;
  gsl_rng *r;
  gsl_rng_env_setup();
  T = gsl_rng_default;
  // creating random generator
  r = gsl_rng_alloc(T);
  // setting the seed
  gsl_rng_set(
    r,
    std::chrono::system_clock::now().time_since_epoch() /
      std::chrono::milliseconds(1));

  // getting sample size with recalculated confidence
  long int sample_size = 0;
  long int sat = 0;
  long int unsat = 0;
  // parameters of the beta distribution
  double alpha = 1;
  double beta = 1;
  // initializing posterior mean
  double post_mean_sat =
    ((double)sat + alpha) / ((double)sample_size + alpha + beta);
  double post_mean_unsat = ((double)sample_size - unsat + alpha) /
                           ((double)sample_size + alpha + beta);
  double post_prob = 0;
  if (global_config.verbose)
    cout << setprecision(16);
  if (global_config.verbose_result)
    cout << "Bayesian estimations algorithm started\n";
  // getting set of all paths
  vector<vector<pdrh::mode *>> paths;
  if (global_config.decision_method == 0)
  {
    paths = pdrh::get_all_paths(min_depth, max_depth);
  }
#pragma omp parallel
  while (post_prob < conf)
  {
    // getting a sample
    box b = rnd::get_random_sample(r);
// increasing the sample size
#pragma omp critical
    {
      sample_size++;
    }
    if (global_config.verbose)
      cout << "Random sample: " << b << "\n";
    std::vector<box> boxes = {b};
    boxes.insert(boxes.end(), nondet_boxes.begin(), nondet_boxes.end());

    int res = decision_procedure::UNDET;
    switch (global_config.decision_method)
    {
    case 0:
      if (global_config.delta_sat)
      {
        res = decision_procedure::evaluate_delta_sat(
          paths, boxes, global_config.solver_bin, global_config.solver_opt);
      }
      else
      {
        res = decision_procedure::evaluate(
          paths, boxes, global_config.solver_bin, global_config.solver_opt);
      }
      break;
    case 1:
      //res = ap::verify(min_depth, max_depth, boxes);
      //break;
    case 2:
      //res = ap::simulate(min_depth, max_depth, boxes);
      //break;
    default:
      cerr << "Unknown decision procedure method" << endl;
      exit(EXIT_FAILURE);
    }
// updating the counters
#pragma omp critical
    {
      switch (res)
      {
      case decision_procedure::SAT:
        if (global_config.verbose)
          cout << "SAT\n";
        sat++;
        break;

      case decision_procedure::UNSAT:
        if (global_config.verbose)
          cout << "UNSAT\n";
        unsat++;
        break;

      case decision_procedure::UNDET:
        if (global_config.verbose)
          cout << "UNDET\n";
        break;
      }
      post_mean_sat =
        ((double)sat + alpha) / ((double)sample_size + alpha + beta);
      post_mean_unsat = ((double)sample_size - unsat + alpha) /
                        ((double)sample_size + alpha + beta);
      if (post_mean_sat >= acc)
      {
        post_prob =
          gsl_cdf_beta_P(
            post_mean_unsat + acc, sample_size - unsat + alpha, unsat + beta) -
          gsl_cdf_beta_P(
            post_mean_sat - acc, sat + alpha, sample_size - sat + beta);
      }
      else
      {
        post_prob =
          gsl_cdf_beta_P(
            post_mean_unsat + acc, sample_size - unsat + alpha, unsat + beta) -
          gsl_cdf_beta_P(0, sat + alpha, sample_size - sat + beta);
      }
      if (global_config.verbose)
      {
        cout << "CI: "
             << capd::interval(
                  max(post_mean_sat - acc, 0.0),
                  min(post_mean_unsat + acc, 1.0))
             << "\n";
        cout << "P(SAT) mean: " << post_mean_sat << "\n";
        cout << "P(UNSAT) mean: " << post_mean_unsat << "\n";
        cout << "Random sample size: " << sample_size << "\n";
        cout << "P prob: " << post_prob << "\n";
      }
    }
  }
  gsl_rng_free(r);
  // displaying sample size if enabled
  if (global_config.verbose_result)
  {
    cout << "Random sample size: " << sample_size << "\n";
    cout << "Bayesian estimations algorithm finished\n";
  }
  return capd::interval(
    max(post_mean_sat - acc, 0.0), min(post_mean_unsat + acc, 1.0));
}

pair<box, capd::interval> algorithm::evaluate_npha_cross_entropy_normal(
  size_t min_depth,
  size_t max_depth,
  size_t size,
  size_t iter_num,
  double acc,
  double conf)
{
  // random number generator for cross entropy
  const gsl_rng_type *T;
  gsl_rng *r;
  gsl_rng_env_setup();
  T = gsl_rng_default;
  // creating random generator
  r = gsl_rng_alloc(T);
  // setting the seed
  gsl_rng_set(
    r,
    std::chrono::system_clock::now().time_since_epoch() /
      std::chrono::milliseconds(1));
  box domain = pdrh::get_nondet_domain();
  //initializing probability value
  pair<box, capd::interval> res(domain, capd::interval(0.0));
  if (global_config.min_prob)
    res = make_pair(domain, capd::interval(1.0));
  if (global_config.verbose)
    cout << setprecision(16);
  if (global_config.verbose_result)
    cout << "Domain of nondeterministic parameters: " << domain << "\n";
  box mean = domain.mid();
  box sigma = domain.get_stddev();
  box var = sigma * sigma;
  vector<pair<box, capd::interval>> samples;
  capd::interval size_correction_coef(1e-32);
  // getting initial mode
  pdrh::mode *init_mode = pdrh::get_mode(pdrh::init.front().id);
  //#pragma omp parallel
  for (int j = 0; j < iter_num; j++)
  {
    var = sigma * sigma;
    if (global_config.verbose_result)
    {
      cout << "Iteration number: " << j + 1 << "\n";
      cout << "Mean: " << mean << "\n";
      cout << "Standard deviation: " << sigma << "\n";
      cout << "Variance: " << var << "\n";
    }
    // correct the sample size only if the probability of sampling outside the domain is still greater than 0.99999
    if (size_correction_coef.leftBound() < 0.99999)
    {
      size_correction_coef = measure::get_sample_prob(domain, mean, sigma);
    }
    unsigned long new_size =
      (unsigned long)ceil(size / size_correction_coef.leftBound());
    if (global_config.verbose_result)
      cout << "Sample size: " << new_size << "\n";
    int outliers = 0;
    //#pragma omp parallel for
    for (int i = 0; i < new_size; i++)
    {
      box b = rnd::get_normal_random_sample(r, mean, sigma);
      if (global_config.verbose_result)
        cout << "Quasi-random sample: " << b << "\n";
      capd::interval probability;
      if (domain.contains(b))
      {
        if (global_config.verbose_result)
          cout << "The sample is inside the domain\n";

        // bayesian estimations algorithm by default
        probability = evaluate_pha_bayesian(
          min_depth, max_depth, acc, conf, vector<box>{b});
        // fixing probability value
        if (probability.leftBound() < 0)
          probability.setLeftBound(0);
        if (probability.rightBound() > 1)
          probability.setRightBound(1);
      }
      else
      {
        if (global_config.verbose_result)
          cout << "The sample is outside the domain\n";

        outliers++;
        if (global_config.min_prob)
          probability = capd::interval(numeric_limits<double>::infinity());
        else
          probability = capd::interval(-numeric_limits<double>::infinity());
      }
      if (global_config.verbose_result)
        cout << "Probability: " << probability << "\n";
      samples.push_back(make_pair(b, probability));
    }
    if (global_config.verbose_result)
      cout << "Number of outliers: " << outliers << "\n";
    if (global_config.min_prob)
    {
      sort(samples.begin(), samples.end(), measure::compare_pairs::ascending);
    }
    else
    {
      sort(samples.begin(), samples.end(), measure::compare_pairs::descending);
    }
    vector<pair<box, capd::interval>> elite;
    copy_n(
      samples.begin(),
      std::max(ceil(samples.size() * global_config.elite_ratio), 2.0),
      back_inserter(elite));
    // getting elite boxes
    vector<box> elite_boxes;
    for (pair<box, capd::interval> p : elite)
    {
      elite_boxes.push_back(p.first);
    }
    // updating resulting probability
    if (global_config.min_prob)
    {
      if (
        samples.front().second.mid().leftBound() < res.second.mid().leftBound())
      {
        res = samples.front();
      }
    }
    else
    {
      if (
        samples.front().second.mid().leftBound() > res.second.mid().leftBound())
      {
        res = samples.front();
      }
    }
    samples.clear();
    mean = box_factory::get_mean(elite_boxes);
    sigma = box_factory::get_stddev(elite_boxes);
  }
  gsl_rng_free(r);
  return res;
}
