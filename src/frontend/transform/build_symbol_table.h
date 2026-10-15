//
// Created by fedor on 14/10/2026.
//

#ifndef PROBREACH_BUILD_SYMBOL_TABLE_H
#define PROBREACH_BUILD_SYMBOL_TABLE_H

#include "symbol_table.h"
#include "ast.h"

class build_symbol_tablet
{
public:
  build_symbol_tablet()
  {
  }

  void build(symbol_tablet &sym_table, modelt *model);

};

#endif // PROBREACH_BUILD_SYMBOL_TABLE_H
