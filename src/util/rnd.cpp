//
// Created by fedor on 04/04/16.
//

#include "rnd.h"
#include "model.h"
#include "node_utils.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <vector>

using namespace std;

box rnd::get_random_sample(gsl_rng *r)
{
  map<std::string, capd::interval> edges;

  // uniform distributions
  for (auto it = old::global_model.sym_table.uniform.cbegin();
       it != old::global_model.sym_table.uniform.cend();
       it++)
  {
    edges.insert(make_pair(
      it->first,
      node_utils::node_to_interval(
        old::global_model.sym_table.uniform[it->first].first) +
        gsl_rng_uniform(r) *
          (node_utils::node_to_interval(
             old::global_model.sym_table.uniform[it->first].second) -
           node_utils::node_to_interval(
             old::global_model.sym_table.uniform[it->first].first))));
  }
  // normal distributions
  for (auto it = old::global_model.sym_table.normal.cbegin();
       it != old::global_model.sym_table.normal.cend();
       it++)
  {
    edges.insert(make_pair(
      it->first,
      node_utils::node_to_interval(
        old::global_model.sym_table.normal[it->first].first) +
        gsl_ran_gaussian_ziggurat(
          r,
          node_utils::node_to_interval(
            old::global_model.sym_table.normal[it->first].second)
            .mid()
            .leftBound())));
  }
  // exponential distributions
  for (auto it = old::global_model.sym_table.exp.cbegin();
       it != old::global_model.sym_table.exp.cend();
       it++)
  {
    edges.insert(make_pair(
      it->first,
      gsl_ran_exponential(
        r,
        1 / node_utils::node_to_interval(old::global_model.sym_table.exp[it->first])
              .mid()
              .leftBound())));
  }
  //discrete distributions
  for (auto it = old::global_model.sym_table.dd_map.cbegin();
       it != old::global_model.sym_table.dd_map.cend();
       it++)
  {
    map<node *, node *> mass_map = old::global_model.sym_table.dd_map[it->first];
    double *p_mass = new double[mass_map.size()];
    node **p_value = new node *[mass_map.size()];
    size_t i = 0;
    // getting values and their probabilities
    for (auto it2 = mass_map.cbegin(); it2 != mass_map.cend(); it2++)
    {
      p_value[i] = it2->first;
      p_mass[i] = node_utils::node_to_interval(it2->second).mid().leftBound();
      i++;
    }
    // getting a pointer to the look up table
    gsl_ran_discrete_t *g = gsl_ran_discrete_preproc(mass_map.size(), p_mass);
    // getting a value index
    size_t index = gsl_ran_discrete(r, g);
    edges.insert(
      std::make_pair(it->first, node_utils::node_to_interval(p_value[index])));
    // releasing memory
    delete[] p_value;
    delete[] p_mass;
    gsl_ran_discrete_free(g);
  }
  return box(edges);
}

box rnd::get_normal_random_sample(gsl_rng *r, box mu, box sigma)
{
  map<std::string, capd::interval> edges;
  for (auto it = old::global_model.sym_table.par_map.cbegin();
       it != old::global_model.sym_table.par_map.cend();
       it++)
  {
    if (it->second.first->value != it->second.second->value)
    {
      if (
        find(mu.get_vars().cbegin(), mu.get_vars().cend(), it->first) !=
          mu.get_vars().cend() &&
        find(sigma.get_vars().cbegin(), sigma.get_vars().cend(), it->first) !=
          sigma.get_vars().cend())
      {
        edges.insert(make_pair(
          it->first,
          mu.get_map()[it->first].mid().leftBound() +
            gsl_ran_gaussian_ziggurat(
              r, sigma.get_map()[it->first].mid().leftBound())));
      }
      else
      {
        cerr << "Parameter \"" << it->first << "\" is not defined\n";
      }
    }
    else
    {
      edges.insert(make_pair(
        it->first,
        node_utils::node_to_interval(it->second.first).mid().leftBound()));
    }
  }
  return box(edges);
}
