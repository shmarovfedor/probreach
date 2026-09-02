//
// Created by fedor on 01/09/2026.
//

#ifndef PROBREACH_IREP_H
#define PROBREACH_IREP_H

#include <string>
#include <ostream>
#include <vector>
#include <memory>
#include <map>

/// General representation of an expression in ProbReach
class exprt
{
public:
  virtual std::string get_type() const = 0;
  virtual ~exprt() = default;
  virtual void print(std::ostream &out) const = 0;

  friend std::ostream &operator<<(std::ostream &os, const exprt &e)
  {
    e.print(os);
    return os;
  }
};

class rvaluet
{
public:
  virtual ~rvaluet() = default;
};

/// Real valued expressions
class real_exprt : public exprt, public rvaluet
{
public:
  virtual ~real_exprt() = default;
};

/// Real valued terminal values
class symbolt : public real_exprt
{
private:
  std::string value;

public:
  symbolt(std::string value) : value(value)
  {
  }

  bool operator==(const symbolt &other) const
  {
    return value == other.value;
  }

  std::string get_type() const override
  {
    return "symbolt";
  }

  std::string get_value()
  {
    return value;
  }

  void print(std::ostream &out) const override
  {
    out << value;
  }
};

class numbert : public real_exprt
{
private:
  std::string value;

public:
  numbert(std::string value) : value(value)
  {
  }

  bool operator==(const numbert &other) const
  {
    return value == other.value;
  }

  std::string get_type() const override
  {
    return "numbert";
  }

  std::string get_value()
  {
    return value;
  }

  void print(std::ostream &out) const override
  {
    out << value;
  }
};

/// Mathematical operations
class minust : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  minust(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "minust";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "( - "
        << "(" << *value << "))";
  }
};

class addt : public real_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  addt(std::unique_ptr<real_exprt> left, std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "addt";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " + " << *right << ")";
  }
};

class subt : public real_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  subt(std::unique_ptr<real_exprt> left, std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "subt";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " - " << *right << ")";
  }
};

class mult : public real_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  mult(std::unique_ptr<real_exprt> left, std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "mult";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " * " << *right << ")";
  }
};

class divt : public real_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  divt(std::unique_ptr<real_exprt> left, std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "divt";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " / " << *right << ")";
  }
};

class powt : public real_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  powt(std::unique_ptr<real_exprt> left, std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "powt";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " ^ " << *right << ")";
  }
};

/// Mathematical functions
class sqrtt : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  sqrtt(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "sqrtt";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "sqrt(" << *value << ")";
  }
};

class abst : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  abst(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "abst";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "abs(" << *value << ")";
  }
};

class sint : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  sint(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "sint";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "sin(" << *value << ")";
  }
};

class cost : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  cost(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "cost";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "cos(" << *value << ")";
  }
};

class tant : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  tant(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "tant";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "tan(" << *value << ")";
  }
};

class asint : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  asint(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "asint";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "asin(" << *value << ")";
  }
};

class acost : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  acost(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "acost";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "acos(" << *value << ")";
  }
};

class atant : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  atant(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "atant";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "atan(" << *value << ")";
  }
};

class expt : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  expt(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "expt";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "exp(" << *value << ")";
  }
};

class logt : public real_exprt
{
private:
  std::unique_ptr<real_exprt> value;

public:
  logt(std::unique_ptr<real_exprt> value) : value(std::move(value))
  {
  }

  std::string get_type() const override
  {
    return "logt";
  }

  real_exprt &get_value()
  {
    return *value;
  }

  void print(std::ostream &out) const override
  {
    out << "log(" << *value << ")";
  }
};

/// Boolean valued expressions
class bool_exprt : public exprt
{
public:
  virtual ~bool_exprt() = default;
};

/// Boolean constants
class truet : public bool_exprt
{
  std::string get_type() const override
  {
    return "truet";
  }

  void print(std::ostream &out) const override
  {
    out << "(true)";
  }
};

class falset : public bool_exprt
{
  std::string get_type() const override
  {
    return "falset";
  }

  void print(std::ostream &out) const override
  {
    out << "(false)";
  }
};

/// Comparison operations
class greater_thant : public bool_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  greater_thant(
    std::unique_ptr<real_exprt> left,
    std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "greater_thant";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " > " << *right << ")";
  }
};

class less_thant : public bool_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  less_thant(
    std::unique_ptr<real_exprt> left,
    std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "less_thant";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " < " << *right << ")";
  }
};

class greater_equalt : public bool_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  greater_equalt(
    std::unique_ptr<real_exprt> left,
    std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "greater_equalt";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " >= " << *right << ")";
  }
};

class less_equalt : public bool_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  less_equalt(
    std::unique_ptr<real_exprt> left,
    std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "less_equalt";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " <= " << *right << ")";
  }
};

class equalt : public bool_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  equalt(std::unique_ptr<real_exprt> left, std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "equalt";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " = " << *right << ")";
  }
};

class not_equalt : public bool_exprt
{
private:
  std::unique_ptr<real_exprt> left;
  std::unique_ptr<real_exprt> right;

public:
  not_equalt(
    std::unique_ptr<real_exprt> left,
    std::unique_ptr<real_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "not_equalt";
  }

  real_exprt &get_left()
  {
    return *left;
  }

  real_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " != " << *right << ")";
  }
};

/// Operations over booleans
class nott : public bool_exprt
{
private:
  std::unique_ptr<bool_exprt> value;

public:
  nott(std::unique_ptr<bool_exprt> value) : value(std::move(value))
  {
  }

  bool_exprt &get_value()
  {
    return *value;
  }

  std::string get_type() const override
  {
    return "nott";
  }

  void print(std::ostream &out) const override
  {
    out << "(not " << *value << ")";
  }
};

class implyt : public bool_exprt
{
private:
  std::unique_ptr<bool_exprt> left;
  std::unique_ptr<bool_exprt> right;

public:
  implyt(std::unique_ptr<bool_exprt> left, std::unique_ptr<bool_exprt> right)
    : left(std::move(left)), right(std::move(right))
  {
  }

  std::string get_type() const override
  {
    return "implyt";
  }

  bool_exprt &get_left()
  {
    return *left;
  }

  bool_exprt &get_right()
  {
    return *right;
  }

  void print(std::ostream &out) const override
  {
    out << "(" << *left << " => " << *right << ")";
  }
};

class andt : public bool_exprt
{
private:
  std::vector<std::unique_ptr<bool_exprt>> operands;

public:
  andt(std::vector<std::unique_ptr<bool_exprt>> operands)
    : operands(std::move(operands))
  {
  }

  std::string get_type() const override
  {
    return "andt";
  }

  std::vector<std::unique_ptr<bool_exprt>> &get_operands()
  {
    return operands;
  }

  void print(std::ostream &out) const override
  {
    out << "(and ";
    for (size_t i = 0; i < operands.size() - 1; i++)
      out << *operands[i] << " ";
    out << *operands.back() << ")";
  }
};

class ort : public bool_exprt
{
private:
  std::vector<std::unique_ptr<bool_exprt>> operands;

public:
  ort(std::vector<std::unique_ptr<bool_exprt>> operands)
    : operands(std::move(operands))
  {
  }

  std::string get_type() const override
  {
    return "ort";
  }

  std::vector<std::unique_ptr<bool_exprt>> &get_operands()
  {
    return operands;
  }

  void print(std::ostream &out) const override
  {
    out << "(or ";
    for (size_t i = 0; i < operands.size() - 1; i++)
      out << *operands[i] << " ";
    out << *operands.back() << ")";
  }
};

class xort : public bool_exprt
{
private:
  std::vector<std::unique_ptr<bool_exprt>> operands;

public:
  xort(std::vector<std::unique_ptr<bool_exprt>> operands)
    : operands(std::move(operands))
  {
  }

  std::string get_type() const override
  {
    return "xort";
  }

  std::vector<std::unique_ptr<bool_exprt>> &get_operands()
  {
    return operands;
  }

  void print(std::ostream &out) const override
  {
    out << "(xor ";
    for (size_t i = 0; i < operands.size() - 1; i++)
      out << *operands[i] << " ";
    out << *operands.back() << ")";
  }
};

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
public:
  virtual std::unique_ptr<real_exprt>
  pdf(const std::unique_ptr<symbolt> sym) const = 0;
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

  std::unique_ptr<real_exprt>
  pdf(const std::unique_ptr<symbolt> sym) const override
  {
    return std::make_unique<divt>(
      std::make_unique<numbert>("1"),
      std::make_unique<subt>(
        std::make_unique<numbert>(*right), std::make_unique<numbert>(*left)));
  }

  std::unique_ptr<real_exprt> pdf()
  {
    return pdf(std::make_unique<symbolt>(""));
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

  std::unique_ptr<real_exprt>
  pdf(const std::unique_ptr<symbolt> sym) const override
  {
    return std::make_unique<mult>(
      std::make_unique<divt>(
        std::make_unique<numbert>("1"),
        std::make_unique<powt>(
          std::make_unique<mult>(
            std::make_unique<numbert>("2"),
            std::make_unique<mult>(
              std::make_unique<numbert>("3.14159"),
              std::make_unique<powt>(
                std::make_unique<numbert>(*sigma),
                std::make_unique<numbert>("2")))),
          std::make_unique<numbert>("0.5"))),
      std::make_unique<expt>(std::make_unique<minust>(std::make_unique<divt>(
        std::make_unique<powt>(
          std::make_unique<subt>(
            std::make_unique<symbolt>(*sym), std::make_unique<numbert>(*mu)),
          std::make_unique<numbert>("2")),
        std::make_unique<mult>(
          std::make_unique<numbert>("2"),
          std::make_unique<powt>(
            std::make_unique<numbert>(*sigma),
            std::make_unique<numbert>("2")))))));
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

  std::unique_ptr<real_exprt>
  pdf(const std::unique_ptr<symbolt> sym) const override
  {
    return std::make_unique<mult>(
      std::make_unique<numbert>(*lambda),
      std::make_unique<expt>(std::make_unique<minust>(std::make_unique<mult>(
        std::make_unique<numbert>(*lambda), std::make_unique<symbolt>(*sym)))));
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
  // Leave implementation for later
};

/// Declarations
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

// Other model components
class odet
{
private:
  std::unique_ptr<symbolt> sym;
  std::unique_ptr<real_exprt> rhs;

public:
  odet(std::unique_ptr<symbolt> sym, std::unique_ptr<real_exprt> rhs)
    : sym(std::move(sym)), rhs(std::move(rhs))
  {
  }

  void print(std::ostream &out) const
  {
    out << "d/dt[" << *sym << "] = " << *rhs;
  }

  symbolt &get_symbol()
  {
    return *sym;
  }

  real_exprt &get_rhs()
  {
    return *rhs;
  }

  friend std::ostream &operator<<(std::ostream &os, const odet &e)
  {
    e.print(os);
    return os;
  }

  std::string get_type() const
  {
    return "odet";
  }
};

class assignt
{
private:
  std::unique_ptr<symbolt> sym;
  std::unique_ptr<rvaluet> rhs;

public:
  assignt(std::unique_ptr<symbolt> sym, std::unique_ptr<rvaluet> rhs)
    : sym(std::move(sym)), rhs(std::move(rhs))
  {
  }

  void print(std::ostream &out) const
  {
    out << *sym << "\' = ";
    if (auto rhs_value = dynamic_cast<real_exprt *>(rhs.get()))
      out << *rhs_value;
    else if (auto rhs_value = dynamic_cast<intervalt *>(rhs.get()))
      out << *rhs_value;
    else if (auto rhs_value = dynamic_cast<distt *>(rhs.get()))
      out << *rhs_value;
  }

  symbolt &get_symbol()
  {
    return *sym;
  }

  rvaluet &get_rhs()
  {
    return *rhs;
  }

  friend std::ostream &operator<<(std::ostream &os, const assignt &e)
  {
    e.print(os);
    return os;
  }

  std::string get_type() const
  {
    return "assignt";
  }
};

class statet
{
protected:
  std::unique_ptr<symbolt> mode_id;

public:
  statet(std::unique_ptr<symbolt> mode_id) : mode_id(std::move(mode_id))
  {
  }

  virtual ~statet() = default;

  symbolt &get_mode_id()
  {
    return *mode_id;
  }

  virtual void print(std::ostream &out) const = 0;
  virtual std::string get_type() const = 0;

  friend std::ostream &operator<<(std::ostream &os, const statet &e)
  {
    e.print(os);
    return os;
  }
};

class cond_statet : public statet
{
private:
  std::unique_ptr<bool_exprt> cond;

public:
  cond_statet(
    std::unique_ptr<symbolt> mode_id,
    std::unique_ptr<bool_exprt> cond)
    : statet(std::move(mode_id)), cond(std::move(cond))
  {
  }

  bool_exprt &get_cond()
  {
    return *cond;
  }

  void print(std::ostream &out) const override
  {
    out << "@" << *mode_id << " " << *cond;
  }

  std::string get_type() const override
  {
    return "cond_statet";
  }
};

class reset_statet : public statet
{
private:
  std::vector<std::unique_ptr<assignt>> assigns;

public:
  reset_statet(
    std::unique_ptr<symbolt> mode_id,
    std::vector<std::unique_ptr<assignt>> assigns)
    : statet(std::move(mode_id)), assigns(std::move(assigns))
  {
  }

  std::string get_type() const override
  {
    return "reset_statet";
  }

  std::vector<std::unique_ptr<assignt>> &get_assignments()
  {
    return assigns;
  }

  void print(std::ostream &out) const override
  {
    out << "(and ";
    for (size_t i = 0; i < assigns.size() - 1; i++)
      out << "(" << *assigns[i] << ") ";
    out << "(" << *assigns.back() << "))";
  }
};

#endif // PROBREACH_IREP_H
