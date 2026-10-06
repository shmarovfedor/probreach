//
// Created by fedor on 29/09/2026.
//

#ifndef PROBREACH_SMT_AST_H
#define PROBREACH_SMT_AST_H

#include <string>
#include <memory>
#include <ostream>

#include "symbol.h"
#include "expr.h"
#include "ast.h"

class smt_nodet
{
public:
  virtual void dump(std::ostream &out) const = 0;
  virtual ~smt_nodet() = default;
};

class smt_declare_funt : public smt_nodet
{
private:
  std::unique_ptr<symbolt> sym;

public:
  smt_declare_funt(std::unique_ptr<symbolt> sym) : sym(std::move(sym))
  {
  }
};

class smt_define_odet : public smt_nodet
{
private:
  std::unique_ptr<flowt> flow;

public:
  smt_define_odet(std::unique_ptr<flowt> flow) : flow(std::move(flow))
  {
  }
};

class smt_integralt : public smt_nodet
{
};

class smt_forallt : public smt_nodet
{
};

class smt_assertt : public smt_nodet
{
private:
  std::unique_ptr<bool_exprt> expr;

public:
  smt_assertt(std::unique_ptr<bool_exprt> expr) : expr(std::move(expr))
  {
  }
};

class smt_formulat
{
private:
  std::vector<std::unique_ptr<smt_nodet>> nodes;

public:
  smt_formulat()
  {
  }

  void push_node(std::unique_ptr<smt_nodet> smt_node);
  void dump(std::ostream &out);
};

#endif // PROBREACH_SMT_AST_H
