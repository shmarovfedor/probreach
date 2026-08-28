//
// Created by fedor on 29/11/18.
//

#ifndef PROBREACH_NAIVE_IVP_H
#define PROBREACH_NAIVE_IVP_H

#include <string>
#include "node.h"
#include "model.h"

namespace naive
{
std::map<std::string, double> init_to_map(model::state);

void output_traj(std::vector<std::map<std::string, double>>, std::ostream &);

std::map<std::string, double> solve_ivp(
  std::map<std::string, node *>,
  std::map<std::string, double>,
  double,
  double);

std::vector<std::map<std::string, double>> trajectory(
  std::map<std::string, node *>,
  std::map<std::string, double>,
  double,
  double);

void simulate(
  std::vector<model::mode>,
  std::vector<model::state>,
  std::vector<model::state>,
  bool,
  size_t,
  size_t,
  size_t,
  double,
  std::ostream &);
} // namespace naive

#endif //PROBREACH_NAIVE_IVP_H
