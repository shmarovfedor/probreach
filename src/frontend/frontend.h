//
// Created by fedor on 18/09/2026.
//

#ifndef PROBREACH_FRONTEND_H
#define PROBREACH_FRONTEND_H

#include "ast.h"

class frontendt
{
private:
  std::unique_ptr<modelt> root;

public:

  frontendt()
  {
  }

  void set_model(std::unique_ptr<modelt> model)
  {
    root = std::move(model);
  }

  modelt &get_model()
  {
    return *root;
  }
  
  int parse(const std::string &filename);
};


#endif // PROBREACH_FRONTEND_H
