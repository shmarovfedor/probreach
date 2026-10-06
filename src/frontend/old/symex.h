//
// Created by fedor on 24/01/16.
//

#ifndef PROBREACH_SYMEX_H
#define PROBREACH_SYMEX_H

#include <vector>
#include <map>
#include <tuple>

#include "node.h"
#include "model.h"

namespace old
{

class symext
{
private:
  modelt model;

public:

  symext(modelt model) : model(model)
  {
  }

  std::vector<modet *> get_successors(modet *);

  std::vector<std::vector<modet *>> get_all_paths(int);
  std::vector<std::vector<modet *>> get_all_paths(int, int);

private:
  std::vector<std::vector<modet *>> get_paths(modet *, modet *, int);
};


}

#endif //PROBREACH_SYMEX_H
