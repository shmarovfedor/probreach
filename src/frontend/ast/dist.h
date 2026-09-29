//
// Created by fedor on 29/09/2026.
//

#ifndef PROBREACH_DIST_H
#define PROBREACH_DIST_H

#include <string>
#include <ostream>
#include <memory>
#include <map>

#include "expr.h"
#include "rvalue.h"

class distt : public rvaluet
{
public:
  virtual std::string get_type() const = 0;
  virtual ~distt() = default;
  virtual void print(std::ostream &out) const = 0;

  friend std::ostream &operator<<(std::ostream &os, const distt &e)
  {
    e.print(os);
    return os;
  }
};

class cont_distt : public distt
{
};

class uniform_distt : public cont_distt
{
private:
  std::unique_ptr<numbert> left;
  std::unique_ptr<numbert> right;

public:
  uniform_distt(std::unique_ptr<numbert> left, std::unique_ptr<numbert> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "uniform_distt";
  }

  numbert &get_left()
  {
    return *left;
  }

  numbert &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "dist_uniform(" << *left << ", " << *right << ")";
  }
};

class normal_distt : public cont_distt
{
private:
  std::unique_ptr<numbert> mu;
  std::unique_ptr<numbert> sigma;

public:
  normal_distt(std::unique_ptr<numbert> mu, std::unique_ptr<numbert> sigma)
    : mu(std::move(mu)), sigma(std::move(sigma))
  {
  }

  std::string get_type() const override
  {
    return "normal_distt";
  }

  numbert &get_mu()
  {
    return *mu;
  }

  numbert &get_sigma()
  {
    return *sigma;
  }

  void print(std::ostream &out) const override
  {
    out << "dist_normal(" << *mu << ", " << *sigma << ")";
  }
};

class exp_distt : public cont_distt
{
private:
  std::unique_ptr<numbert> lambda;

public:
  exp_distt(std::unique_ptr<numbert> lambda) : lambda(std::move(lambda))
  {
  }

  std::string get_type() const override
  {
    return "exp_distt";
  }

  numbert &get_lambda()
  {
    return *lambda;
  }

  void print(std::ostream &out) const override
  {
    out << "dist_exp(" << *lambda << ")";
  }
};

class gamma_distt : public cont_distt
{
  // Leave implementation for later
};

class beta_distt : public cont_distt
{
  // Leave implementation for later
};

class discrete_distt : public distt
{
private:
  std::map<std::unique_ptr<numbert>, std::unique_ptr<numbert>> p_mass;

public:
  discrete_distt(
    std::map<std::unique_ptr<numbert>, std::unique_ptr<numbert>> p_mass)
    : p_mass(std::move(p_mass))
  {
  }

  std::string get_type() const override
  {
    return "discrete_distt";
  }

  std::map<std::unique_ptr<numbert>, std::unique_ptr<numbert>> &get_p_mass()
  {
    return p_mass;
  }

  void print(std::ostream &out) const override
  {
    out << "dist_discrete(";
    for (auto it = p_mass.cbegin(); it != std::prev(p_mass.cend()); ++it)
      out << *(it->first) << ":" << *(it->second) << ", ";
    out << *(std::prev(p_mass.cend())->first) << ":"
        << *(std::prev(p_mass.cend())->second) << ")";
  }
};

#endif // PROBREACH_DIST_H
