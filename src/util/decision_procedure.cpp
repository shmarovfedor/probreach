//
// Created by fedor on 26/02/16.
//

#include <unistd.h>
#include <omp.h>
#include "smt2_generator.h"
#include "decision_procedure.h"
#include "dreal_wrapper.h"
#include "pdrh_config.h"

using namespace std;

/**
 * Evaluates reachability for a set of paths and parameter boxes.
 *
 * @param paths - set of paths to evaluate.
 * @param boxes - set of parameter boxes to evaluate.
 * @param solver_bin - full path to solver binary.
 * @param solver_opt - string of solver options.
 * @return satisfiability of reachability for the given set of paths and parameter boxes.
 */
int decision_procedure::evaluate(
  vector<vector<pdrh::mode *>> paths,
  vector<box> boxes,
  string solver_bin,
  string solver_opt)
{
  int undet_counter = 0;
  for (vector<pdrh::mode *> path : paths)
  {
    int res = evaluate(path, boxes, solver_bin, solver_opt);
    if (res == decision_procedure::result::SAT)
    {
      return res;
    }
    else if (res == decision_procedure::result::UNDET)
    {
      undet_counter++;
    }
  }
  if (undet_counter > 0)
  {
    return decision_procedure::result::UNDET;
  }
  return decision_procedure::result::UNSAT;
}

/**
 * Evaluates reachability for a path and a set of parameter boxes.
 *
 * @param path - path to evaluate.
 * @param boxes - set of parameter boxes to evaluate.
 * @param solver_bin - full path to solver binary.
 * @param solver_opt - string of solver options.
 * @return satisfiability of reachability for the given path and parameter boxes.
 */
int decision_procedure::evaluate(
  vector<pdrh::mode *> path,
  vector<box> boxes,
  string solver_bin,
  string solver_opt)
{
  // evaluating the delta-sat formula
  int first_res = decision_procedure::evaluate_delta_sat(
    path, boxes, global_config.solver_bin, solver_opt);
  if (first_res == decision_procedure::result::UNSAT)
  {
    return decision_procedure::result::UNSAT;
  }
  else if (first_res == decision_procedure::result::SAT)
  {
    // evaluating complement
    int second_res = decision_procedure::evaluate_complement(
      path, boxes, solver_bin, solver_opt);
    if (second_res == decision_procedure::result::UNSAT)
    {
      return decision_procedure::result::SAT;
    }
    else if (second_res == decision_procedure::result::SAT)
    {
      return decision_procedure::result::UNDET;
    }
  }
}

/**
 * Evaluates delta-reachability for a set of paths and parameter boxes.
 *
 * @param paths - set of paths to evaluate.
 * @param boxes - set of parameter boxes to evaluate.
 * @param solver_bin - full path to solver binary.
 * @param solver_opt - string of solver options.
 * @return satisfiability of reachability for the given set of paths and parameter boxes.
 */
int decision_procedure::evaluate_delta_sat(
  vector<vector<pdrh::mode *>> paths,
  vector<box> boxes,
  string solver_bin,
  string solver_opt)
{
  int undet_counter = 0;
  for (vector<pdrh::mode *> path : paths)
  {
    int res = evaluate_delta_sat(path, boxes, solver_bin, solver_opt);
    if (res == decision_procedure::result::SAT)
    {
      return res;
    }
  }
  return decision_procedure::result::UNSAT;
}

/**
 * Evaluates delta-reachability for a path and a set of parameter boxes.
 *
 * @param path - path to evaluate.
 * @param boxes - set of parameter boxes to evaluate.
 * @param solver_bin - full path to solver binary.
 * @param solver_opt - string of solver options.
 * @return delta-satisfiability of reachability for the given path and parameter boxes.
 */
int decision_procedure::evaluate_delta_sat(
  vector<pdrh::mode *> path,
  vector<box> boxes,
  string solver_bin,
  string solver_opt)
{
  // default value for the thread number
  int thread_num = 0;
#ifdef _OPENMP
  thread_num = omp_get_thread_num();
#endif
  // getting raw filename here
  std::string filename = std::string(global_config.model_filename);
  size_t ext_index = filename.find_last_of('.');
  std::string raw_filename = filename.substr(0, ext_index);
  // creating a name for the smt2 file
  std::stringstream f_stream;
  f_stream << raw_filename << "_" << path.size() - 1 << "_0_" << thread_num
           << ".smt2";
  std::string smt_filename = f_stream.str();
  // writing to the file
  std::ofstream smt_file;

  smt_file.open(smt_filename.c_str());
  // will work for one initial and one state only
  smt_file << smt2_generator::reach_to_smt2(path, boxes);
  smt_file.close();

  if (global_config.debug)
  {
    cout << "Thread: " << omp_get_thread_num() << endl;
    cout << "First formula:" << endl;
    cout << smt2_generator::reach_to_smt2(path, boxes) << endl;
  }

  int first_res = dreal::execute(solver_bin, smt_filename, solver_opt);

  if (global_config.debug)
    cout << "dReal result = " << first_res << "\n";

  if (first_res == -1)
  {
    return decision_procedure::ERROR;
  }
  else if (first_res == 1)
  {
    if (
      (std::remove(smt_filename.c_str()) == 0) &&
      (std::remove(std::string(smt_filename + ".output").c_str()) == 0))
    {
      return decision_procedure::UNSAT;
    }
    else
    {
      cerr << "Problem occurred while removing one of auxiliary files (UNSAT)";
      return decision_procedure::ERROR;
    }
  }
  else
  {
    if (
      (std::remove(smt_filename.c_str()) == 0) &&
      (std::remove(std::string(smt_filename + ".output").c_str()) == 0))
    {
      return decision_procedure::SAT;
    }
    else
    {
      cerr
        << "Problem occurred while removing one of auxiliary files (DELTA-SAT)";
      return decision_procedure::ERROR;
    }
  }
}

/**
 * Evaluates delta-non-reachability for a path and a set of parameter boxes.
 *
 * @param path - path to evaluate.
 * @param boxes - set of parameter boxes to evaluate.
 * @param solver_bin - full path to solver binary.
 * @param solver_opt - string of solver options.
 * @return delta-satisfiability of non-reachability for the given path and parameter boxes.
 */
int decision_procedure::evaluate_complement(
  vector<pdrh::mode *> path,
  vector<box> boxes,
  string solver_bin,
  string solver_opt)
{
  int thread_num = 0;
#ifdef _OPENMP
  thread_num = omp_get_thread_num();
#endif
  // getting raw filename here
  string filename = string(global_config.model_filename);
  size_t ext_index = filename.find_last_of('.');
  string raw_filename = filename.substr(0, ext_index);
  // creating a name for the smt2 file
  stringstream f_stream;
  for (int i = 0; i < path.size(); i++)
  {
    // the complement formula
    f_stream.str("");
    f_stream << raw_filename << "_" << i << "_" << path.size() - 1 << "_0_"
             << thread_num << ".c.smt2";
    string smt_c_filename = f_stream.str();
    // writing to the file
    ofstream smt_c_file;
    smt_c_file.open(smt_c_filename.c_str());
    smt_c_file << smt2_generator::reach_c_to_smt2(i, path, boxes);
    if (global_config.debug)
    {
      cout << "Thread: " << omp_get_thread_num() << endl;
      cout << "Second formula (" << i << "):" << endl;
      cout << smt2_generator::reach_c_to_smt2(i, path, boxes) << endl;
    }
    smt_c_file.close();
    // calling dreal here
    int second_res = dreal::execute(solver_bin, smt_c_filename, solver_opt);

    if (global_config.debug)
      cout << "dReal result = " << second_res << "\n";

    if (second_res == -1)
    {
      return decision_procedure::ERROR;
    }
    else if (second_res == 1)
    {
      if (
        (remove(smt_c_filename.c_str()) != 0) ||
        (remove(std::string(smt_c_filename + ".output").c_str()) != 0))
      {
        return decision_procedure::ERROR;
      }
    }
    else
    {
      if (
        (remove(smt_c_filename.c_str()) != 0) ||
        (remove(std::string(smt_c_filename + ".output").c_str()) != 0))
      {
        return decision_procedure::ERROR;
      }
      else
      {
        return decision_procedure::SAT;
      }
    }
  }
  return decision_procedure::UNSAT;
}
