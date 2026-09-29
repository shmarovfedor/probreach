//
// Created by fedor on 01/09/2026.
//

#ifndef PROBREACH_DECL_H
#define PROBREACH_DECL_H

#include <string>
#include <ostream>
#include <memory>

#include "symbol.h"
#include "expr.h"
#include "dist.h"
#include "interval.h"

class declt
{
protected:
  std::unique_ptr<symbolt> sym;

public:
  declt(std::unique_ptr<symbolt> sym) : sym(std::move(sym))
  {
  }

  virtual std::string get_type() const = 0;
  virtual ~declt() = default;
  virtual void print(std::ostream &out) const = 0;

  symbolt &get_symbol()
  {
    return *sym;
  }

  friend std::ostream &operator<<(std::ostream &os, const declt &e)
  {
    e.print(os);
    return os;
  }
};

class const_declt : public declt
{
private:
  std::unique_ptr<numbert> value;

public:
  const_declt(std::unique_ptr<symbolt> sym, std::unique_ptr<numbert> value)
    : declt(std::move(sym)), value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "const_declt";
  }

  void print(std::ostream &out) const override
  {
    out << "[" << *value << "] " << *sym;
  }

  numbert &get_value()
  {
    return *value;
  }
};

class var_declt : public declt
{
private:
  std::unique_ptr<intervalt> domain;

public:
  var_declt(std::unique_ptr<symbolt> sym, std::unique_ptr<intervalt> domain)
    : declt(std::move(sym)), domain(std::move(domain))
  {
  }

  std::string get_type() const override
  {
    return "var_declt";
  }

  void print(std::ostream &out) const override
  {
    out << *domain << " " << *sym;
  }

  intervalt &get_domain()
  {
    return *domain;
  }
};

class dist_declt : public declt
{
private:
  std::unique_ptr<distt> dist;

public:
  dist_declt(std::unique_ptr<symbolt> sym, std::unique_ptr<distt> dist)
    : declt(std::move(sym)), dist(std::move(dist))
  {
  }

  std::string get_type() const override
  {
    return "dist_declt";
  }

  void print(std::ostream &out) const override
  {
    out << *dist << " " << *sym;
  }

  distt &get_dist()
  {
    return *dist;
  }
};

#endif // PROBREACH_DECL_H
