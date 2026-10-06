//
// Created by fedor on 02/03/17.
//

#ifndef PROBREACH_SOLVER_H
#define PROBREACH_SOLVER_H

#include <string>

class solvert
{
private:
  std::string solver_bin_path;

public:
  solvert(std::string solver_bin_path) : solver_bin_path(solver_bin_path)
  {
  }

  int run(std::string file_path, std::string args);
};

#endif //PROBREACH_SOLVER_H
