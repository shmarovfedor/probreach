//
// Created by fedor on 27/12/15.
//
#include <capd/capdlib.h>
#include "measure.h"
#include "box_factory.h"
#include "model.h"
#include "pdrh_config.h"
#include "node_utils.h"
#include <iomanip>

using namespace std;
using namespace pdrh;

std::pair<capd::interval, std::vector<capd::interval>>
measure::integral(std::string var, std::string fun, capd::interval it, double e)
{
  std::vector<capd::interval> stack, partition;
  capd::interval value(0);
  stack.push_back(it);
  while (!stack.empty())
  {
    capd::interval i = stack.front();
    stack.erase(stack.begin());
    capd::interval v[] = {i};
    capd::IVector x(1, v);
    capd::IJet jet(1, 1, 4);
    capd::IMap f_map("var:" + var + ";fun:" + fun + ";");
    f_map.setDegree(4);
    f_map(x, jet);
    capd::interval f4 = jet(0, 3) * 24;
    capd::IFunction fun_fun("var:" + var + ";fun:" + fun + ";");
    capd::interval itg = (capd::intervals::width(i) / 6) *
                           (fun_fun(i.leftBound()) + 4 * fun_fun(i.mid()) +
                            fun_fun(i.rightBound())) -
                         (power(capd::intervals::width(i), 5) / 2880) * f4;
    if (
      capd::intervals::width(itg) <=
      e * (capd::intervals::width(i) / capd::intervals::width(it)))
    {
      partition.push_back(i);
      value += itg;
    }
    else
    {
      stack.push_back(capd::interval(i.leftBound(), i.mid().rightBound()));
      stack.push_back(capd::interval(i.mid().leftBound(), i.rightBound()));
    }
  }
  return make_pair(value, partition);
}

double measure::precision(double e, int n)
{
  double xi = e;
  double lb = 0;
  double ub = e;
  // 100 iterations is more than enough for n = 100
  for (int i = 0; i < 100; i++)
  {
    if (pow((1 + xi), n) - 1 <= e)
    {
      lb = xi;
      xi = (ub + xi) / 2;
    }
    else
    {
      ub = xi;
      xi = (lb + xi) / 2;
    }
  }
  return xi;
}

capd::interval measure::p_measure(box b, double e)
{
  map<std::string, capd::interval> edges = b.get_map();
  capd::interval res(1.0);
  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    if (pdrh::rv_map.find(it->first) != pdrh::rv_map.cend())
    {
      res *= measure::integral(
               it->first,
               std::get<0>(pdrh::rv_map[it->first])->to_infix(),
               it->second,
               measure::precision(e, edges.size()))
               .first;
    }
    else if (pdrh::dd_map.find(it->first) != pdrh::dd_map.cend())
    {
      bool measure_exists = false;
      map<node *, node *> tmp_map = pdrh::dd_map[it->first];
      for (auto it2 = tmp_map.cbegin(); it2 != tmp_map.cend(); it2++)
      {
        if (it->second == pdrh::node_to_interval(it2->first))
        {
          res *= pdrh::node_to_interval(it2->second);
          measure_exists = true;
          break;
        }
      }
      if (!measure_exists)
      {
        std::stringstream s;
        s << "Measure for " << it->first << " = " << it->second
          << " is undefined";
        throw std::invalid_argument(s.str());
      }
    }
    else
    {
      std::stringstream s;
      s << "Measure for " << it->first << " is undefined";
      throw std::invalid_argument(s.str());
    }
  }
  return res;
}

capd::interval measure::p_measure(box b)
{
  return p_measure(b, global_config.precision_prob);
}

capd::interval measure::p_dd_measure(box b)
{
  std::map<std::string, capd::interval> edges = b.get_map();
  capd::interval res(1.0);
  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    if (pdrh::dd_map.find(it->first) != pdrh::dd_map.cend())
    {
      bool measure_exists = false;
      map<node *, node *> tmp_map = pdrh::dd_map[it->first];
      for (auto it2 = tmp_map.cbegin(); it2 != tmp_map.cend(); it2++)
      {
        if (it->second == pdrh::node_to_interval(it2->first))
        {
          res *= pdrh::node_to_interval(it2->second);
          measure_exists = true;
          break;
        }
      }
      if (!measure_exists)
      {
        std::stringstream s;
        s << "Measure for " << it->first << " = " << it->second
          << " is undefined";
        throw std::invalid_argument(s.str());
      }
    }
    else
    {
      std::stringstream s;
      s << "Measure for " << it->first << " is undefined";
      throw std::invalid_argument(s.str());
    }
  }
  return res;
}

std::string
measure::gaussian_pdf(std::string var, capd::interval mu, capd::interval sigma)
{
  std::stringstream s;
  // outputs only 16 numbers. This is a temporary solution. Will need to declare
  // numbers in scientific notation as parameters
  s.precision(16);
  s << fixed;
  s << "(1 / (" << sigma.leftBound()
    << " * sqrt(2 * 3.14159265359)) * exp(- (( " << var << " - ("
    << mu.leftBound() << ")) * (" << var << " - (" << mu.leftBound()
    << "))) / (2 * (" << sigma.leftBound() << ") * (" << sigma.leftBound()
    << "))))";
  return s.str();
}

capd::interval measure::get_sample_prob(box domain, box mean, box sigma)
{
  if (!box_factory::compatible({domain, mean, sigma}))
  {
    std::stringstream s;
    throw std::invalid_argument(s.str());
  }
  map<string, capd::interval> edges = domain.get_map();
  capd::interval res(1.0);
  for (auto it = edges.begin(); it != edges.end(); it++)
  {
    // considering only the parameters which domain is not a single point
    if (
      pdrh::par_map[it->first].first->value !=
      pdrh::par_map[it->first].second->value)
    {
      double prec = 1e-5;
      //double prec = sigma.get_map()[it->first].leftBound() / 10;
      pair<capd::interval, vector<capd::interval>> itg = measure::integral(
        it->first,
        measure::gaussian_pdf(
          it->first, mean.get_map()[it->first], sigma.get_map()[it->first]),
        it->second,
        prec);
      res *= itg.first;
    }
  }
  return res;
}

std::pair<capd::interval, std::vector<capd::interval>> measure::bounds_from_pdf(
  std::string var,
  std::string pdf,
  capd::interval domain,
  double start,
  double e)
{
  // checking if the starting point is in the domain
  if (!domain.contains(start))
  {
    std::stringstream s;
    s << "starting point " << start << " does not belong to the domain "
      << domain << " while trying to find pdf bounds";
    throw std::invalid_argument(s.str());
  }
  // initializing the interval
  capd::interval res = capd::interval(start);
  while (true)
  {
    // setting the interval
    res = capd::interval(
      res.leftBound() - global_config.integral_pdf_step,
      res.rightBound() + global_config.integral_pdf_step);
    //std::cout << res << std::endl;
    // adjusting left bound of the initial interval
    if (res.leftBound() < domain.leftBound())
    {
      res = capd::interval(domain.leftBound(), res.rightBound());
    }
    // adjusting right bound of the initial interval
    if (res.rightBound() > domain.rightBound())
    {
      res = capd::interval(res.leftBound(), domain.rightBound());
    }
    // calculating integral
    //cout << "BEFORE INTEGRAL" << endl;
    std::pair<capd::interval, std::vector<capd::interval>> itg =
      measure::integral(var, pdf, res, e);
    // checking if the value of the integral satisfies the condition
    if (1 - itg.first.leftBound() < e * global_config.integral_inf_coeff)
    {
      return make_pair(res, itg.second);
    }
    // the bound was reached but the precision value is not reached
    else if (res == domain)
    {
      std::stringstream s;
      s << "Unable to bound the integral of the pdf on " << domain
        << " by the value " << e * global_config.integral_inf_coeff;
      throw std::out_of_range(s.str());
    }
  }
}

std::vector<box> measure::get_rv_partition()
{
  std::map<std::string, std::vector<capd::interval>> partition_map;
  for (auto it = pdrh::rv_map.cbegin(); it != pdrh::rv_map.cend(); it++)
  {
    // setting initial rv bounds
    capd::interval init_domain(
      -numeric_limits<double>::infinity(), numeric_limits<double>::infinity());
    if (get<1>(it->second)->value != "-infty")
    {
      init_domain.setLeftBound(
        pdrh::node_to_interval(std::get<1>(it->second)).leftBound());
    }
    if (get<2>(it->second)->value != "infty")
    {
      init_domain.setRightBound(
        pdrh::node_to_interval(std::get<2>(it->second)).rightBound());
    }
    // getting rv bounds
    std::pair<capd::interval, std::vector<capd::interval>> bound =
      measure::bounds_from_pdf(
        it->first,
        get<0>(it->second)->to_infix(),
        init_domain,
        pdrh::node_to_interval(get<3>(it->second)).mid().leftBound(),
        measure::precision(global_config.precision_prob, pdrh::rv_map.size()));
    // updating rv bounds
    pdrh::rv_map[it->first] = make_tuple(
      std::get<0>(it->second),
      new node(bound.first.leftBound()),
      new node(bound.first.rightBound()),
      get<3>(it->second));
    // updating var bounds
    pdrh::var_map[it->first] = make_pair(
      new node(bound.first.leftBound()), new node(bound.first.rightBound()));
    // updating partition map
    partition_map.insert(make_pair(it->first, bound.second));
  }
  return box_factory::cartesian_product(partition_map);
}

std::vector<box> measure::get_dd_partition()
{
  std::map<std::string, std::vector<capd::interval>> m;
  for (auto it = pdrh::dd_map.cbegin(); it != pdrh::dd_map.cend(); it++)
  {
    std::vector<capd::interval> args;
    for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); it2++)
    {
      args.push_back(pdrh::node_to_interval(it2->first));
    }
    m.insert(make_pair(it->first, args));
  }
  return box_factory::cartesian_product(m);
}

box measure::get_rv_domain()
{
  map<std::string, vector<capd::interval>> domain_map;
  for (auto it = pdrh::rv_map.cbegin(); it != pdrh::rv_map.cend(); it++)
  {
    vector<capd::interval> tmp;
    tmp.push_back(capd::interval(
      pdrh::node_to_interval(get<1>(it->second)).leftBound(),
      pdrh::node_to_interval(get<2>(it->second)).rightBound()));
    domain_map.insert(std::make_pair(it->first, tmp));
  }
  if (domain_map.empty())
  {
    return box();
  }
  std::vector<box> domain = box_factory::cartesian_product(domain_map);
  return domain.front();
}

// comparing the medians of the intervals
bool measure::compare_pairs::ascending(
  const pair<box, capd::interval> &lhs,
  const pair<box, capd::interval> &rhs)
{
  if (
    !box_factory::get_keys_diff(lhs.first, rhs.first).empty() ||
    !box_factory::get_keys_diff(rhs.first, lhs.first).empty())
  {
    stringstream s;
    s << "Boxes " << lhs.first << " and " << rhs.first << " cannot be compared";
    throw std::invalid_argument(s.str());
  }
  return lhs.second.mid() < rhs.second.mid();
}

// comparing the medians of the intervals
bool measure::compare_pairs::descending(
  const pair<box, capd::interval> &lhs,
  const pair<box, capd::interval> &rhs)
{
  if (
    !box_factory::get_keys_diff(lhs.first, rhs.first).empty() ||
    !box_factory::get_keys_diff(rhs.first, lhs.first).empty())
  {
    stringstream s;
    s << "Boxes " << lhs.first << " and " << rhs.first << " cannot be compared";
    throw std::invalid_argument(s.str());
  }
  return lhs.second.mid() > rhs.second.mid();
}

bool measure::compare_boxes_by_p_measure(const box lhs, const box rhs)
{
  return measure::p_measure(lhs).mid().leftBound() >
         measure::p_measure(rhs).mid().leftBound();
}
