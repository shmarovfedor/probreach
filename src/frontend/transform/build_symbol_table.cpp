#include "symbol_table.h"
#include "build_symbol_table.h"

void build_symbol_tablet::build(symbol_tablet &sym_table, modelt *model)
{
  for(auto it = model->get_declarations().cbegin();
      it != model->get_declarations().cend(); ++it)
  {
    if(!sym_table.insert(it->second->get_symbol(), it->second.get()))
    {
      throw std::runtime_error("duplicate declaration of symbol \"" + 
          it->second->get_symbol().get_value() + "\"");
    }
  }
}

