//
// Created by fedor on 06/11/17.
//

#ifndef PROBREACH_SMT2_GENERATOR_H
#define PROBREACH_SMT2_GENERATOR_H

#include <iostream>
#include "model.h"
#include "box.h"

namespace smt2_generator
{

// generates reachability formulas
std::string reach_to_smt2(std::vector<pdrh::mode *>, std::vector<box>);
std::string reach_c_to_smt2(std::vector<pdrh::mode *>, std::vector<box>);
std::string reach_c_to_smt2(int, std::vector<pdrh::mode *>, std::vector<box>);

} // namespace smt2_generator

#endif //PROBREACH_SMT2_GENERATOR_H
