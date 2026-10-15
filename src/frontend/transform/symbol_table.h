//
// Created by fedor on 14/10/2026.
//

#ifndef PROBREACH_SYMBOL_TABLE_H
#define PROBREACH_SYMBOL_TABLE_H

#include <map>
#include <string>
#include "symbol.h"
#include "decl.h"

class symbol_tablet
{
private:
  std::map<symbolt, declt*> sym_table;

public:
  symbol_tablet() 
  {
  }

  using iterator = std::map<symbolt, declt*>::iterator;
  using const_iterator = std::map<symbolt, declt*>::const_iterator;

  bool insert(const symbolt& sym, declt* decl)
  {
    return sym_table.emplace(sym, decl).second;
  }

  const declt* lookup(const symbolt &sym) const
  {
    auto it = sym_table.find(sym);
    return (it == sym_table.end()) ? nullptr : it->second;
  }

  iterator begin() { return sym_table.begin(); }
  iterator end() { return sym_table.end(); }

  const_iterator begin() const { return sym_table.begin(); }
  const_iterator end() const { return sym_table.end(); }

  const_iterator cbegin() const { return sym_table.cbegin(); }
  const_iterator cend() const { return sym_table.cend(); }

};

#endif // PROBREACH_SYMBOL_TABLE_H
