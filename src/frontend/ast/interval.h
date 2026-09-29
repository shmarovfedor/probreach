//
// Created by fedor on 29/09/2026.
//

#ifndef PROBREACH_INTERVAL_H
#define PROBREACH_INTERVAL_H

#include <string>
#include <ostream>
#include <memory>

#include "rvalue.h"

/// Intervals and distributions
class intervalt : public rvaluet
{
private:
  std::unique_ptr<numbert> left;
  std::unique_ptr<numbert> right;

public:
  intervalt(std::unique_ptr<numbert> left, std::unique_ptr<numbert> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  intervalt(const intervalt &other)
    : left(other.left ? std::make_unique<numbert>(*other.left) : nullptr),
      right(other.right ? std::make_unique<numbert>(*other.right) : nullptr)
  {
  }

  std::string get_type() const
  {
    return "intervalt";
  }

  numbert &get_left()
  {
    return *left;
  }

  numbert &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const
  {
    out << "[" << *left << ", " << *right << "]";
  }

  friend std::ostream &operator<<(std::ostream &os, const intervalt &e)
  {
    e.print(os);
    return os;
  }
};

#endif // PROBREACH_INTERVAL_H
