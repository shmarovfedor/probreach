//
// Created by fedor on 06/11/17.
//

#ifndef PROBREACH_SMT2_GENERATOR_H
#define PROBREACH_SMT2_GENERATOR_H

#include <iostream>
#include "model.h"
#include "box.h"

class smt2_generatort
{

public:
// generates reachability formulas
  static std::string reach_to_smt2(std::vector<old::modet *>, std::vector<box>);
  static std::string reach_c_to_smt2(std::vector<old::modet *>, std::vector<box>);
  static std::string reach_c_to_smt2(int, std::vector<old::modet *>, std::vector<box>);

};

#endif //PROBREACH_SMT2_GENERATOR_H
