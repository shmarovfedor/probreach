#include "trace.h"
#include "build_traces.h"

#include <iostream>

tracet *build_tracest::next()
{
  // currently returned trace is valid; we need to get the next one
  if(trace_pending)
  {
    trace_pending = false;
    stack.pop_back();
    if(!current_trace.is_empty())
      current_trace.pop_transition();
  }
    
  while(true)
  {
    // initialise the next {init, goal} pair
    if(stack.empty())
    {
      if(!initialise_next_init_goal_pair())
        return nullptr;
    }

    auto &frame = stack.back();

    // reached the specified depth
    if(current_trace.get_depth() == depth)
    {
      // found a valid trace
      if(frame.current_mode->get_symbol() ==
         current_trace.get_goal()->get_mode_id())
      {
        trace_pending = true;
        return &current_trace;
      }

      // reached the depth but the last mode is not the goal mode;
      // need to backtrack
      stack.pop_back();
      if(!current_trace.is_empty())
        current_trace.pop_transition();

      continue;
    }

    const auto &successors = graph.get_successors(frame.current_mode);

    // the current mode is a terminal mode; need to backtrack
    if(successors.at(0)->is_terminal())
    {
      stack.pop_back();

      if(!current_trace.is_empty())
        current_trace.pop_transition();

      continue;
    }

    // explored the last successor at the current depth; need to backtrack
    if(frame.next_successor >= successors.size())
    {
      stack.pop_back();

      if(!current_trace.is_empty())
        current_trace.pop_transition();

      continue;
    }

    // finally, can explore the next successor
    const transitiont *transition = successors[frame.next_successor++];
    current_trace.push_transition(transition);
    stack.push_back({transition->get_target(), 0});
  }
}

bool build_tracest::initialise_next_init_goal_pair()
{
  stack.clear();
  while(init_idx < model.get_inits().size())
  {
    cond_statet *init = model.get_inits()[init_idx].get();

    const modet *init_mode =
      dynamic_cast<const modet *>(sym_table.lookup(init->get_mode_id()));

    while(goal_idx < model.get_goals().size())
    {
      cond_statet *goal = model.get_goals()[goal_idx++].get();

      current_trace = tracet{};
      current_trace.set_init(init);
      current_trace.set_goal(goal);

      stack.push_back({init_mode, 0});

      return true;
    }

    goal_idx = 0;
    ++init_idx;
  }

  return false;
}








