//
// Created by fedor on 19/10/26.
//
#ifndef PROBREACH_TRANSITION_GRAPH_H
#define PROBREACH_TRANSITION_GRAPH_H

#include "ast.h"

class path_nodet
{
private:
  const modet *source;
  const jumpt *jump;
  const modet *target;

public:
  path_nodet(modet *source, jumpt *jump, modet *target)
    : source(source), jump(jump), target(target)
  {
  }

}


#endif //PROBREACH_TRANSITION_GRAPH_H
