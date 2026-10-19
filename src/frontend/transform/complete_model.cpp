#include <algorithm>
#include <iostream>

#include "complete_model.h"

void complete_modelt::run(symbol_tablet &sym_table, modelt &model)
{
  for (const auto &mode : model.get_modes())
  {
    visit(sym_table, *mode);
  }
}

void complete_modelt::visit(symbol_tablet &sym_table, modet &mode)
{
  visit(sym_table, mode.get_flow());
  for (const auto &jump : mode.get_jumps())
  {
    visit(sym_table, *jump);
  }
}

void complete_modelt::visit(symbol_tablet &sym_table, flowt &flow)
{
  std::vector<std::unique_ptr<odet>> &odes = flow.get_odes();
  std::vector<symbolt> ode_syms;
  for (auto it = odes.cbegin(); it != odes.cend(); ++it)
    ode_syms.push_back((*it)->get_symbol());

  for (auto it = sym_table.cbegin(); it != sym_table.cend(); ++it)
  {
    if (std::find(ode_syms.cbegin(), ode_syms.cend(), it->first) 
        == ode_syms.cend() && it->second->get_type() != "modet")
    {
      std::unique_ptr<symbolt> sym = std::make_unique<symbolt>(it->first);
      std::unique_ptr<numbert> rhs = std::make_unique<numbert>("0.0");
      flow.insert(std::make_unique<odet>(std::move(sym), std::move(rhs)));
    }
  }
}

void complete_modelt::visit(symbol_tablet &sym_table, jumpt &jump)
{
  visit(sym_table, jump.get_reset());
}

void complete_modelt::visit(symbol_tablet &sym_table, reset_statet &reset)
{
  std::vector<std::unique_ptr<assignt>> &assigns = reset.get_assignments();
  std::vector<symbolt> assign_syms;
  for (auto it = assigns.cbegin(); it != assigns.cend(); ++it)
    assign_syms.push_back((*it)->get_symbol());

  for (auto it = sym_table.cbegin(); it != sym_table.cend(); ++it)
  {
    if (std::find(assign_syms.cbegin(), assign_syms.cend(), it->first) 
        == assign_syms.cend() && it->second->get_type() != "modet")
    {
      std::unique_ptr<symbolt> sym = std::make_unique<symbolt>(it->first);
      std::unique_ptr<symbol_exprt> rhs = 
        std::make_unique<symbol_exprt>(sym->get_value());
      reset.insert(std::make_unique<assignt>(std::move(sym), std::move(rhs)));
    }
  }
}








