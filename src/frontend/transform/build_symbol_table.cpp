#include "symbol_table.h"
#include "build_symbol_table.h"

void build_symbol_tablet::build(symbol_tablet &sym_table, modelt *model)
{
  // variables declarations
  for(const auto &decl : model->get_declarations())
  {
    if(!sym_table.insert(decl->get_symbol(), decl.get()))
    {
      throw std::runtime_error("duplicate declaration of symbol \"" + 
          decl->get_symbol().get_value() + "\"");
    }
  }
  // modes declarations
  for(const auto &mode : model->get_modes())
  {
    if(!sym_table.insert(mode->get_symbol(), mode.get()))
    {
      throw std::runtime_error("duplicate declaration of symbol \"" + 
          mode->get_symbol().get_value() + "\"");
    }
  }
}

