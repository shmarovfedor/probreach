//
// Created by fedor on 19/10/26.
//
#ifndef PROBREACH_TRANSITION_GRAPH_H
#define PROBREACH_TRANSITION_GRAPH_H

#include <unordered_map>

#include "ast.h"

class transitiont
{
private:
  const modet *source;
  const jumpt *jump;
  const modet *target;

public:
  transitiont(const modet *source, const jumpt *jump, const modet *target)
    : source(source), jump(jump), target(target)
  {
  }

  const modet *get_source() const
  {
    return source;
  }

  const jumpt *get_jump() const
  {
    return jump;
  }

  const modet *get_target() const
  {
    return target;
  }
  
  void print(std::ostream &out) const
  {
    out << "@" << source->get_symbol() << " " << *jump;
  }

  friend std::ostream &operator<<(std::ostream &os, const transitiont &e)
  {
    e.print(os);
    return os;
  }
};

class transition_grapht
{
private:
  std::vector<std::unique_ptr<transitiont>> transitions;
  std::unordered_map<const modet *, std::vector<const transitiont *>> outgoing;

public:
  void add_transition(
      const modet *source, 
      const jumpt *jump, 
      const modet *target)
  {
    auto transition =
        std::make_unique<transitiont>(
            transitiont{
                source,
                jump,
                target
            });

    auto *ptr = transition.get();
    transitions.push_back(std::move(transition));
    outgoing[source].push_back(ptr);
  }

  const std::vector<const transitiont *> & 
    get_outgoing(const modet *mode) const;
  
  void print(std::ostream &out) const
  {
    for (auto &it : transitions)
      out << *it << "\n";
  }

  friend std::ostream &operator<<(std::ostream &os, const transition_grapht &e)
  {
    e.print(os);
    return os;
  }
};

#endif //PROBREACH_TRANSITION_GRAPH_H
