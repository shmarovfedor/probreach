//
// Created by fedor on 26/02/16.
//

#include <fstream>
#include <stdexcept>
#include <sstream>
#include <chrono>

#include "dreal_solver.h"
#include "box.h"

using namespace std;

const vector<string> SAT_ANSWERS = {"sat", "delta-sat"};
const vector<string> UNSAT_ANSWERS = {"unsat"};

solver_resultt dreal_solvert::run(std::string file_path, std::string args) const
{
  stringstream s;
  s << get_bin_path() << " " 
    << args << " " 
    << file_path << " > " 
    << file_path << ".output";

  auto start = std::chrono::steady_clock::now(); 
  int exit_code = system(s.str().c_str());
  auto end = std::chrono::steady_clock::now(); 
  
  double cpu_time_seconds = std::chrono::duration<double>(end - start).count();

  if (exit_code != 0)
  {
    stringstream e;
    e << "Unexpected problem running: " << s.str();
    throw runtime_error(e.str());
  }
  
  fstream output_file;
  std::string output = file_path + ".output";
  output_file.open(output.c_str());
  if (!output_file.is_open())
  {
    stringstream s;
    s << "Problem opening the file " << output;
    throw runtime_error(s.str());
  }

  // getting the last line of the file
  string last_line, line;
  while (getline(output_file, line))
  {
    last_line = line;
  }
  output_file.close();
  // analyzing the last line of the output
  string res;
  unsigned long pos = last_line.find_first_of(" ");
  if (pos != string::npos)
  {
    res = last_line.substr(0, pos);
  }
  else
  {
    res = last_line;
  }
  resultt result;
  // checking if the output line is a sat answer
  if (find(SAT_ANSWERS.begin(), SAT_ANSWERS.end(), res) != SAT_ANSWERS.end())
  {
    result = resultt::SAT;
  }
  else if (
    find(UNSAT_ANSWERS.begin(), UNSAT_ANSWERS.end(), res) !=
    UNSAT_ANSWERS.end())
  // checking if the output line is an unsat answer
  {
    result = resultt::UNSAT;
  }
  else
  // unrecognized answer
  {
    stringstream s;
    s << "Unrecognized solver output: " << res;
    throw runtime_error(s.str());
  }

  return solver_resultt(result, cpu_time_seconds, exit_code);
}
