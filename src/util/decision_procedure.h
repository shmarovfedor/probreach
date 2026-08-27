//
// Created by fedor on 26/02/16.
//

#ifndef PROBREACH_DECISION_PROCEDURE_H
#define PROBREACH_DECISION_PROCEDURE_H
#include <iostream>
#include "model.h"
#include "box.h"

namespace decision_procedure
{
enum result
{
  SAT,
  UNSAT,
  UNDET,
  ERROR
};

// evaluating all paths
int evaluate(
  std::vector<std::vector<pdrh::mode *>>,
  std::vector<box>,
  std::string,
  std::string);

// evaluates a path
int evaluate(
  std::vector<pdrh::mode *>,
  std::vector<box>,
  std::string,
  std::string);

// first argument is the path to be evaluated,
// second argument is the set of nondet boxes
// third argument is the path to the solver binary
// fourt argument is the string of solver options
int evaluate_delta_sat(
  std::vector<pdrh::mode *>,
  std::vector<box>,
  std::string,
  std::string);

// first argument is the path to be evaluated,
// second argument is the set of nondet boxes
// third argument is the path to the solver binary
// fourt argument is the string of solver options
int evaluate_delta_sat(
  std::vector<std::vector<pdrh::mode *>>,
  std::vector<box>,
  std::string,
  std::string);

// first argument is the path to be evaluated,
// second argument is the set of nondet boxes
// third argument is the path to the solver binary
// fourt argument is the string of solver options
int evaluate_complement(
  std::vector<pdrh::mode *>,
  std::vector<box>,
  std::string,
  std::string);

} // namespace decision_procedure

#endif //PROBREACH_DECISION_PROCEDURE_H
