//
// Created by fedor on 27/12/15.
//
#include <capd/capdlib.h>
#include <capd/intervals/lib.h>

#include "box.h"
#include "symbol_table.h"

#ifndef PROBREACH_MEASURER_H
#define PROBREACH_MEASURER_H

class measurert
{
private:
  old::symbol_tablet sym_table;

public:
  measurert(old::symbol_tablet sym_table);

  capd::interval measure(box b, double precision);

  // obtain the partition of the parameter space
  std::vector<box> get_rv_partition();
  std::vector<box> get_dd_partition();
  box get_rv_domain();
  box get_nondet_domain();

  capd::interval get_sample_prob(box, box, box);

  std::pair<capd::interval, std::vector<capd::interval>> integral(
      std::string var, 
      std::string fun, 
      capd::interval bounds, 
      double precision);

  std::pair<capd::interval, std::vector<capd::interval>> bounds_from_pdf(
      std::string var, 
      std::string pdf, 
      capd::interval domain, 
      double start,
      double step,
      double inf_cutoff,
      double precision);

  static double precision(double, int);
  static std::string
    gaussian_pdf(std::string var, capd::interval mu, capd::interval sigma);
};

namespace compare_pairs
{
bool ascending(
  const std::pair<box, capd::interval> &,
  const std::pair<box, capd::interval> &);
bool descending(
  const std::pair<box, capd::interval> &,
  const std::pair<box, capd::interval> &);
} // namespace compare_pairs


#endif //PROBREACH_MEASURER_H
