//
// Created by fedor on 04/04/16.
//

#ifndef PROBREACH_RANDOM_H
#define PROBREACH_RANDOM_H

#include <box.h>
#include <gsl/gsl_rng.h>

namespace rnd
{
box get_random_sample(gsl_rng *);
box get_normal_random_sample(gsl_rng *, box, box);
} // namespace rnd

#endif //PROBREACH_RANDOM_H
