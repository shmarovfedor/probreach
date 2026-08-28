//
// Created by fedor on 26/02/16.
//

#ifndef PROBREACH_DREAL_WRAPPER_H
#define PROBREACH_DREAL_WRAPPER_H

#include "box.h"

namespace dreal
{
extern std::string solver_bin;
extern std::string cmd_args;

int execute(std::string, std::string, std::string);
box parse_model(std::string);
} // namespace dreal

#endif //PROBREACH_DREAL_WRAPPER_H
