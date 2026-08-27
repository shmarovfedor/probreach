//
// Created by fedor on 27/12/15.
//
#include <capd/capdlib.h>
#include <capd/intervals/lib.h>
#include "box.h"

#ifndef PROBREACH_MEASURE_H
#define PROBREACH_MEASURE_H

namespace measure
{
std::pair<capd::interval, std::vector<capd::interval>>
integral(std::string, std::string, capd::interval, double);

capd::interval p_measure(box, double);
capd::interval p_measure(box);
capd::interval p_dd_measure(box);

capd::interval get_sample_prob(box, box, box);

namespace compare_pairs
{
bool ascending(
  const std::pair<box, capd::interval> &,
  const std::pair<box, capd::interval> &);
bool descending(
  const std::pair<box, capd::interval> &,
  const std::pair<box, capd::interval> &);
} // namespace compare_pairs

bool compare_boxes_by_p_measure(const box, const box);

// obtain the partition of the parameter space
std::vector<box> get_rv_partition();
std::vector<box> get_dd_partition();
box get_rv_domain();
box get_nondet_domain();

double precision(double, int);

std::pair<capd::interval, std::vector<capd::interval>>
bounds_from_pdf(std::string, std::string, capd::interval, double, double);

std::string
gaussian_pdf(std::string var, capd::interval mu, capd::interval sigma);

} // namespace measure

#endif //PROBREACH_MEASURE_H
