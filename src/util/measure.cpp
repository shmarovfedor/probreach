//
// Created by fedor on 27/12/15.
//
#include <capd/capdlib.h>
#include "measure.h"
#include "box_factory.h"
#include "pdrh_config.h"
#include "node_utils.h"

using namespace std;

measurert::measurert(old::symbol_tablet sym_table) 
{
  this->sym_table = sym_table;
}

std::pair<capd::interval, std::vector<capd::interval>> measurert::integral(
    std::string var, 
    std::string fun, 
    capd::interval it, 
    double precision)
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
      precision * (capd::intervals::width(i) / capd::intervals::width(it)))
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

double measurert::precision(double e, int n)
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

capd::interval measurert::measure(box b, double precision)
{
  map<std::string, capd::interval> edges = b.get_map();
  capd::interval res(1.0);
  for (auto it = edges.cbegin(); it != edges.cend(); it++)
  {
    if (sym_table.rv_map.find(it->first) != sym_table.rv_map.cend())
    {
      res *= measurert::integral(
               it->first,
               std::get<0>(sym_table.rv_map[it->first])->to_infix(),
               it->second,
               measurert::precision(precision, edges.size()))
               .first;
    }
    else if (sym_table.dd_map.find(it->first) != sym_table.dd_map.cend())
    {
      bool measure_exists = false;
      map<node *, node *> tmp_map = sym_table.dd_map[it->first];
      for (auto it2 = tmp_map.cbegin(); it2 != tmp_map.cend(); it2++)
      {
        if (it->second == node_utils::node_to_interval(it2->first))
        {
          res *= node_utils::node_to_interval(it2->second);
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
measurert::gaussian_pdf(std::string var, capd::interval mu, capd::interval sigma)
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

capd::interval measurert::get_sample_prob(box domain, box mean, box sigma)
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
    if (sym_table.par_map[it->first].first->value !=
      sym_table.par_map[it->first].second->value)
    {
      double prec = 1e-5;
      //double prec = sigma.get_map()[it->first].leftBound() / 10;
      pair<capd::interval, vector<capd::interval>> itg = measurert::integral(
        it->first,
        measurert::gaussian_pdf(
          it->first, mean.get_map()[it->first], sigma.get_map()[it->first]),
        it->second,
        prec);
      res *= itg.first;
    }
  }
  return res;
}

std::pair<capd::interval, std::vector<capd::interval>> measurert::bounds_from_pdf(
  std::string var,
  std::string pdf,
  capd::interval domain,
  double start,
  double step,
  double inf_cutoff,
  double precision)
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
      res.leftBound() - step,
      res.rightBound() + step);
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
    std::pair<capd::interval, std::vector<capd::interval>> itg =
      measurert::integral(var, pdf, res, precision);
    // checking if the value of the integral satisfies the condition
    if (1 - itg.first.leftBound() < precision * inf_cutoff)
    {
      return make_pair(res, itg.second);
    }
    // the bound was reached but the precision value is not reached
    else if (res == domain)
    {
      std::stringstream s;
      s << "Unable to bound the integral of the pdf on " << domain
        << " by the value " << precision * global_config.integral_inf_coeff;
      throw std::out_of_range(s.str());
    }
  }
}

std::vector<box> measurert::get_rv_partition()
{
  std::map<std::string, std::vector<capd::interval>> partition_map;
  for (auto it = sym_table.rv_map.cbegin(); it != sym_table.rv_map.cend(); ++it)
  {
    // setting initial rv bounds
    capd::interval init_domain(
      -numeric_limits<double>::infinity(), numeric_limits<double>::infinity());
    if (get<1>(it->second)->value != "-infty")
    {
      init_domain.setLeftBound(
        node_utils::node_to_interval(std::get<1>(it->second)).leftBound());
    }
    if (get<2>(it->second)->value != "infty")
    {
      init_domain.setRightBound(
        node_utils::node_to_interval(std::get<2>(it->second)).rightBound());
    }
    // getting rv bounds
    std::pair<capd::interval, std::vector<capd::interval>> bound =
      measurert::bounds_from_pdf(
        it->first,
        get<0>(it->second)->to_infix(),
        init_domain,
        node_utils::node_to_interval(get<3>(it->second)).mid().leftBound(),
        global_config.integral_pdf_step,
        global_config.integral_inf_coeff,
        measurert::precision(
          global_config.precision_prob, sym_table.rv_map.size()));
    // updating partition map
    partition_map.insert(make_pair(it->first, bound.second));
  }
  return box_factory::cartesian_product(partition_map);
}

// domain of continuous random parameters
box measurert::get_rv_domain()
{
  map<std::string, vector<capd::interval>> domain_map;
  for (auto it = sym_table.rv_map.cbegin(); it != sym_table.rv_map.cend(); ++it)
  {
    // setting initial rv bounds
    capd::interval init_domain(
      -numeric_limits<double>::infinity(), numeric_limits<double>::infinity());
    if (get<1>(it->second)->value != "-infty")
    {
      init_domain.setLeftBound(
        node_utils::node_to_interval(std::get<1>(it->second)).leftBound());
    }
    if (get<2>(it->second)->value != "infty")
    {
      init_domain.setRightBound(
        node_utils::node_to_interval(std::get<2>(it->second)).rightBound());
    }
    // getting rv bounds
    std::pair<capd::interval, std::vector<capd::interval>> bound =
      measurert::bounds_from_pdf(
        it->first,
        get<0>(it->second)->to_infix(),
        init_domain,
        node_utils::node_to_interval(get<3>(it->second)).mid().leftBound(),
        global_config.integral_pdf_step,
        global_config.integral_inf_coeff,
        measurert::precision(
          global_config.precision_prob, sym_table.rv_map.size()));
    domain_map.insert(
        std::make_pair(it->first, std::vector<capd::interval>({bound.first})));
  }
  if (domain_map.empty())
  {
    return box();
  }
  std::vector<box> domain = box_factory::cartesian_product(domain_map);
  return domain.front();
}

std::vector<box> measurert::get_dd_partition()
{
  std::map<std::string, std::vector<capd::interval>> m;
  for (auto it = sym_table.dd_map.cbegin(); it != sym_table.dd_map.cend(); ++it)
  {
    std::vector<capd::interval> args;
    for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); it2++)
    {
      args.push_back(node_utils::node_to_interval(it2->first));
    }
    m.insert(make_pair(it->first, args));
  }
  return box_factory::cartesian_product(m);
}

// domain of nondeterministic parameters
box measurert::get_nondet_domain()
{
  map<std::string, capd::interval> m;
  for (auto it = sym_table.par_map.cbegin(); 
      it != sym_table.par_map.cend(); 
      ++it)
  {
    m.insert(make_pair(
      it->first,
      capd::interval(
        node_utils::node_to_interval(it->second.first).leftBound(),
        node_utils::node_to_interval(it->second.second).rightBound())));
  }
  return box(m);
}

// comparing the medians of the intervals
bool compare_pairs::ascending(
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
bool compare_pairs::descending(
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
