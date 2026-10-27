//
// Created by fedor on 02/03/17.
//

#ifndef PROBREACH_SOLVER_H
#define PROBREACH_SOLVER_H

#include <string>

enum class resultt
{
  SAT,
  UNSAT,
  UNKNOWN,
  TIMEOUT,
  ERROR
};

inline std::ostream &operator<<(std::ostream &os, resultt result)
{
  switch(result)
  {
  case resultt::SAT:
    return os << "SAT";
  case resultt::UNSAT:
    return os << "UNSAT";
  case resultt::TIMEOUT:
    return os << "TIMEOUT";
  case resultt::ERROR:
    return os << "ERROR";
  case resultt::UNKNOWN:
    return os << "UNKNOWN";
  }
}

class solver_resultt
{
private:
  resultt result;
  double cpu_time_seconds;
  int exit_code;

public:
  solver_resultt(resultt result, double cpu_time_seconds, int exit_code)
    : result(result), cpu_time_seconds(cpu_time_seconds), exit_code(exit_code)
  {
  }

  resultt get_result()
  {
    return result;
  }

  double get_cpu_time()
  {
    return cpu_time_seconds;
  }

  int get_exit_code()
  {
    return exit_code;
  }

  void print(std::ostream &out) const
  {
    out << result << " (EXIT CODE: " << exit_code << ") " 
      << cpu_time_seconds << "s";
  }

  friend std::ostream &operator<<(std::ostream &os, const solver_resultt &e)
  {
    e.print(os);
    return os;
  }

};

class solvert
{
private:
  const std::string solver_bin_path;

public:
  solvert(std::string solver_bin_path) : solver_bin_path(solver_bin_path)
  {
  }

  virtual solver_resultt run(std::string file_path, std::string args) const = 0;

  const std::string get_bin_path() const
  {
    return solver_bin_path;
  }
};

#endif //PROBREACH_SOLVER_H
