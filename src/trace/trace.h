//
// Created by fedor on 23/10/26.
//
#ifndef PROBREACH_TRACE_H
#define PROBREACH_TRACE_H

#include <vector>

#include "ast.h"
#include "transition_graph.h"

class tracet
{
private:
  cond_statet *init = nullptr;
  cond_statet *goal = nullptr;
  std::vector<const transitiont*> transitions;

public:
  tracet()
  {
  }

  void set_init(cond_statet *init)
  {
    this->init = init;
  }

  cond_statet *get_init()
  {
    return this->init;
  }

  void set_goal(cond_statet *goal)
  {
    this->goal = goal;
  }

  cond_statet *get_goal()
  {
    return this->goal;
  }

  void push_transition(const transitiont *transition)
  {
    transitions.push_back(transition);
  }

  void pop_transition()
  {
    transitions.pop_back();
  }

  size_t get_depth()
  {
    return transitions.size();
  }

  const transitiont* at(size_t idx)
  {
    return transitions.at(idx);
  }

  bool is_empty() const
  {
    return transitions.empty();
  }

  void print(std::ostream &out) const
  {
    out << *init << "\n";
    out << "    |\n";
    out << "    v\n";
    if (is_empty())
    {
      out << "   [X]\n";
      out << "    |\n";
      out << "    v\n";
      out << *goal << "\n";
    }
    else
    {
      for(auto &it : transitions)
      {
        out << "    " << *it << "\n";
        out << "    |\n";
        out << "    v\n";
      }
      out << *goal << "\n";
    }
  }

  friend std::ostream &operator<<(std::ostream &os, const tracet &e)
  {
    e.print(os);
    return os;
  }
};

#endif //PROBREACH_TRACE_H
