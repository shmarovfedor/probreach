#include "build_transition_graph.h"

void build_transition_grapht::build(transition_grapht &graph) const
{
  for(auto &[symbol, decl] : symbol_table)
  {
    auto *source = dynamic_cast<modet *>(decl);
    if(source == nullptr)
      continue;

    for(auto &jump : source->get_jumps())
    {
      const modet *target = resolve_target(*jump);
      graph.add_transition(source, jump.get(), target);
    }
  }

  return graph;
}

const modet *build_transition_grapht::resolve_target(jumpt &jump) const
{
  symbolt &target_symbol = jump.get_reset().get_mode_id();
  return lookup_mode(target_symbol);
}

const modet *build_transition_grapht::lookup_mode(
    symbolt &symbol) const
{
  auto it = symbol_table.lookup(symbol);
  if(it == nullptr)
  {
    throw std::runtime_error(
            "unknown mode `" +
            symbol.get_value() +
            "`");
  }

  auto *mode = dynamic_cast<const modet *>(it);
  if(mode == nullptr)
  {
    throw std::runtime_error(
            "`" +
            symbol.get_value() +
            "` is not a mode");
  }

  return mode;
}
