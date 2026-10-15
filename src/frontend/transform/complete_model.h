//
// Created by fedor on 15/10/2026.
//

#ifndef PROBREACH_COMPLETE_MODEL_H
#define PROBREACH_COMPLETE_MODEL_H

#include "symbol_table.h"
#include "ast.h"

class complete_modelt
{
public:
  complete_modelt()
  {
  }

  void run(symbol_tablet &sym_table, modelt &model);

private:
  void visit(symbol_tablet &sym_table, modet &flow);
  void visit(symbol_tablet &sym_table, flowt &flow);
  void visit(symbol_tablet &sym_table, jumpt &jump);
  void visit(symbol_tablet &sym_table, reset_statet &reset);

};

#endif // PROBREACH_COMPLETE_MODEL_H
