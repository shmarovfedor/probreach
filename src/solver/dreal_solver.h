//
// Created by fedor on 26/02/16.
//

#ifndef PROBREACH_DREAL_SOLVER_H
#define PROBREACH_DREAL_SOLVER_H

#include <string>

#include "box.h"
#include "solver.h"

class dreal_solvert : public solvert
{
public:

  dreal_solvert(std::string solver_bin_path) : solvert(solver_bin_path)
  {
  }

  int run(std::string file_path, std::string args) const override;

};

#endif //PROBREACH_DREAL_SOLVER_H
