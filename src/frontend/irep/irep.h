//
// Created by fedor on 01/09/2026.
//

#ifndef PROBREACH_IREP_H
#define PROBREACH_IREP_H

#include <string>
#include <ostream>
#include <vector>
#include <memory>

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

/// Real valued expressions
class real_exprt : public exprt
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
    out << "( - " << "(" << *value << "))";
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
  equalt(
    std::unique_ptr<real_exprt> left,
    std::unique_ptr<real_exprt> right)
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
  implyt(
    std::unique_ptr<bool_exprt> left,
    std::unique_ptr<bool_exprt> right)
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
  andt(
    std::vector<std::unique_ptr<bool_exprt>> operands)
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
  ort(
    std::vector<std::unique_ptr<bool_exprt>> operands)
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
  xort(
    std::vector<std::unique_ptr<bool_exprt>> operands)
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

#endif // PROBREACH_IREP_H
