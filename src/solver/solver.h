//
// Created by fedor on 02/03/17.
//

#ifndef PROBREACH_SOLVER_H
#define PROBREACH_SOLVER_H

#include <string>

class solvert
{
private:
  const std::string solver_bin_path;

public:
  solvert(std::string solver_bin_path) : solver_bin_path(solver_bin_path)
  {
  }

  virtual int run(std::string file_path, std::string args) const = 0;

  const std::string get_bin_path() const
  {
    return solver_bin_path;
  }
};

#endif //PROBREACH_SOLVER_H
