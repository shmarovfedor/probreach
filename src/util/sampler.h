//
// Created by fedor on 04/04/16.
//

#ifndef PROBREACH_SAMPLER_H
#define PROBREACH_SAMPLER_H

#include <gsl/gsl_rng.h>

#include "box.h"
#include "symbol_table.h"

class samplert
{
private:
  old::symbol_tablet sym_table;
  gsl_rng *r;

public:

  samplert(old::symbol_tablet sym_table);

  ~samplert() 
  {
    gsl_rng_free(r);
  }

  box get_random_sample();
  box get_normal_random_sample(box mu, box sigma);
};

#endif //PROBREACH_SAMPLER_H
