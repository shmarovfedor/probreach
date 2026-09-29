//
// Created by fedor on 29/09/2026.
//

#ifndef PROBREACH_ASSIGN_H
#define PROBREACH_ASSIGN_H

#include <string>
#include <memory>
#include <ostream>

#include "symbol.h"
#include "rvalue.h"

class assignt
{
private:
  std::unique_ptr<symbolt> sym;
  std::unique_ptr<rvaluet> rhs;

public:
  assignt(std::unique_ptr<symbolt> sym, std::unique_ptr<rvaluet> rhs)
    : sym(std::move(sym)), rhs(std::move(rhs))
  {
  }

  void print(std::ostream &out) const
  {
    out << *sym << "\' = ";
    if (auto rhs_value = dynamic_cast<real_exprt *>(rhs.get()))
      out << *rhs_value;
    else if (auto rhs_value = dynamic_cast<intervalt *>(rhs.get()))
      out << *rhs_value;
    else if (auto rhs_value = dynamic_cast<distt *>(rhs.get()))
      out << *rhs_value;
  }

  symbolt &get_symbol()
  {
    return *sym;
  }

  rvaluet &get_rhs()
  {
    return *rhs;
  }

  friend std::ostream &operator<<(std::ostream &os, const assignt &e)
  {
    e.print(os);
    return os;
  }

  std::string get_type() const
  {
    return "assignt";
  }
};

#endif // PROBREACH_ASSIGN_H
