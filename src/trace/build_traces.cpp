#include "trace.h"
#include "build_traces.h"

#include <iostream>

tracet *build_tracest::next()
{
  size_t it_counter = 0;
  while(true)
  {
    // First call
    if(stack.empty())
    {
      if(!initialise_next_init_goal_pair())
        return nullptr;
    }
   
    /*
    std::cout << "Current trace (" << it_counter++ << "):\n";
    std::cout << current_trace << "\n";
    */

    auto &top_frame = stack.back();

    // Goal reached?
    if(!top_frame.transition->is_terminal() && 
        top_frame.transition->get_target()->get_symbol() == 
        current_trace.get_goal()->get_mode_id() && 
        current_trace.get_depth() == depth)
    {
      tracet result = current_trace;

      stack.pop_back();
      if(!current_trace.is_empty())
        current_trace.pop_transition();

      current_trace = result;
      return &current_trace;
    }

    // Depth bound
    if(stack.size() > depth)
    {
      stack.pop_back();

      if(!current_trace.is_empty())
        current_trace.pop_transition();

      continue;
    }
    
    if(top_frame.transition->is_terminal())
    {
      stack.pop_back();

      if(!current_trace.is_empty())
        current_trace.pop_transition();

      continue;
    }

    const auto &successors = 
      graph.get_successors(top_frame.transition->get_target());

    if(top_frame.next_successor >= successors.size())
    {
      stack.pop_back();

      if(!current_trace.is_empty())
        current_trace.pop_transition();

      continue;
    }

    const transitiont *transition = successors.at(top_frame.next_successor);
    ++top_frame.next_successor;

    current_trace.push_transition(transition);

    stack.push_back({transition, 0});
  }
}

bool build_tracest::initialise_next_init_goal_pair()
{
  stack.clear();

  while(init_idx < model.get_inits().size())
  {
    cond_statet *init = model.get_inits().at(init_idx).get();
    const modet* init_mode = 
      dynamic_cast<const modet*>(sym_table.lookup(init->get_mode_id()));
    std::vector<const transitiont *> successors = 
      graph.get_successors(init_mode);
    current_trace = tracet{};
    current_trace.push_transition(successors.at(0));
    current_trace.set_init(init);
    stack.push_back({successors.at(0), 0});
    while(goal_idx < model.get_goals().size())
    {
      cond_statet *goal = model.get_goals().at(goal_idx).get();
      ++goal_idx;
      current_trace.set_goal(goal);
      return true;
    }
    goal_idx = 0;
    ++init_idx;
  }

  return false;
}
