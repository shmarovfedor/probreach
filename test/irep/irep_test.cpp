//
// Created by fedor on 23/01/17.
//

#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

#include "irep.h"

using namespace std;

/**
 * Testing symbolt
 */
TEST(symbolt_ostream, normal_test)
{
  string sym_name("a1");
  symbolt sym(sym_name);
  stringstream s;
  s << sym;
  EXPECT_EQ(s.str(), sym_name);
}

TEST(symbolt_get_type, normal_test)
{
  symbolt sym("a1");
  EXPECT_EQ(sym.get_type(), "symbolt");
}

TEST(symbolt_equals, normal_test)
{
  symbolt sym("a1");
  symbolt sym2("a1");
  symbolt sym3("b1");
  EXPECT_TRUE(sym == sym2);
  EXPECT_FALSE(sym == sym3);
}

TEST(symbolt_get_value, normal_test)
{
  string sym_name("a1");
  symbolt sym(sym_name);
  EXPECT_EQ(sym.get_value(), sym_name);
}

/**
 * Testing numbert
 */
TEST(numbert_ostream, normal_test)
{
  string num_val("0.456771");
  numbert num(num_val);
  stringstream s;
  s << num;
  EXPECT_EQ(s.str(), num_val);
}

TEST(numbert_get_type, normal_test)
{
  numbert num("0.456771");
  EXPECT_EQ(num.get_type(), "numbert");
}

TEST(numbert_equals, normal_test)
{
  numbert num("0.456771");
  numbert num2("0.456771");
  numbert num3("3.1415");
  EXPECT_TRUE(num == num2);
  EXPECT_FALSE(num == num3);
}

TEST(numbert_get_value, normal_test)
{
  string num_val("0.456771");
  numbert num(num_val);
  EXPECT_EQ(num.get_value(), num_val);
}

TEST(addt_ostream, normal_test)
{
  std::unique_ptr<addt> add = std::make_unique<addt>(
    std::make_unique<symbolt>("a1"), std::make_unique<numbert>("0.456771"));

  auto add2 =
    std::make_unique<addt>(std::move(add), std::make_unique<numbert>("3.1415"));

  stringstream s;
  s << *add2;

  EXPECT_EQ(s.str(), "((a1 + 0.456771) + 3.1415)");

  auto left = dynamic_cast<addt *>(&(add2->get_left()));
  EXPECT_NE(left, nullptr);
  EXPECT_EQ(left->get_type(), "addt");

  s.str("");
  s.clear();
  s << *left;
  EXPECT_EQ(s.str(), "(a1 + 0.456771)");

  auto right = dynamic_cast<numbert *>(&(add2->get_right()));
  EXPECT_NE(right, nullptr);
  EXPECT_EQ(right->get_type(), "numbert");

  s.str("");
  s.clear();
  s << *right;
  EXPECT_EQ(s.str(), "3.1415");
}

TEST(greater_thant_ostream, normal_test)
{
  auto add = std::make_unique<addt>(
    std::make_unique<symbolt>("a1"), std::make_unique<numbert>("0.456771"));

  auto add2 = std::make_unique<addt>(
    std::make_unique<symbolt>("b_2"), std::make_unique<numbert>("3.1415"));

  auto gt = std::make_unique<greater_thant>(std::move(add), std::move(add2));

  stringstream s;
  s << *gt;
  EXPECT_EQ(s.str(), "((a1 + 0.456771) > (b_2 + 3.1415))");
}

TEST(andt_ostream, normal_test)
{
  auto expr1 = std::make_unique<greater_equalt>(
    std::make_unique<addt>(
      std::make_unique<symbolt>("a1"), std::make_unique<numbert>("0.456771")),
    std::make_unique<addt>(
      std::make_unique<symbolt>("b_2"), std::make_unique<numbert>("3.1415")));

  auto expr2 = std::make_unique<less_equalt>(
    std::make_unique<addt>(
      std::make_unique<symbolt>("a1"), std::make_unique<numbert>("0.456771")),
    std::make_unique<addt>(
      std::make_unique<symbolt>("b_2"), std::make_unique<numbert>("3.1415")));

  std::vector<std::unique_ptr<bool_exprt>> operands;
  operands.push_back(std::move(expr1));
  operands.push_back(std::move(expr2));
  operands.push_back(std::make_unique<truet>());

  auto and_expr = std::make_unique<andt>(std::move(operands));

  stringstream s;
  s << *and_expr;
  EXPECT_EQ(
    s.str(),
    "(and ((a1 + 0.456771) >= (b_2 + 3.1415)) ((a1 + 0.456771) <= (b_2 + "
    "3.1415)) (true))");
}

TEST(intrevalt_ostream, normal_test)
{
  auto intreval = std::make_unique<intervalt>(
    std::make_unique<numbert>("-0.176"), std::make_unique<numbert>("0.456771"));

  stringstream s;
  s << *intreval;
  EXPECT_EQ(s.str(), "[-0.176, 0.456771]");
}

TEST(uniform_distt_ostream, normal_test)
{
  auto u_dist = std::make_unique<uniform_distt>(
    std::make_unique<numbert>("0.176"), std::make_unique<numbert>("0.456771"));

  stringstream s;
  s << *u_dist;
  EXPECT_EQ(s.str(), "dist_uniform(0.176, 0.456771)");

  s.str("");
  s.clear();

  s << *(u_dist->pdf());
  EXPECT_EQ(s.str(), "(1 / (0.456771 - 0.176))");
}

TEST(normal_distt_ostream, normal_test)
{
  auto n_dist = std::make_unique<normal_distt>(
    std::make_unique<numbert>("0.176"), std::make_unique<numbert>("0.456771"));

  stringstream s;
  s << *n_dist;
  EXPECT_EQ(s.str(), "dist_normal(0.176, 0.456771)");

  s.str("");
  s.clear();

  s << *(n_dist->pdf(std::make_unique<symbolt>("p_12")));
  EXPECT_EQ(
    s.str(),
    "((1 / ((2 * (3.14159 * (0.456771 ^ 2))) ^ 0.5)) * exp(( - ((((p_12 - "
    "0.176) ^ 2) / (2 * (0.456771 ^ 2)))))))");
}

TEST(exp_distt_ostream, normal_test)
{
  auto e_dist = std::make_unique<exp_distt>(
    std::make_unique<numbert>("0.176"));

  stringstream s;
  s << *e_dist;
  EXPECT_EQ(s.str(), "dist_exp(0.176)");

  s.str("");
  s.clear();

  s << *(e_dist->pdf(std::make_unique<symbolt>("p_12")));
  EXPECT_EQ(s.str(), "(0.176 * exp(( - ((0.176 * p_12)))))");
}

TEST(const_declt_ostream, normal_test)
{
  auto const1 = std::make_unique<const_declt>(
    std::make_unique<symbolt>("pi"), std::make_unique<numbert>("3.1415"));

  stringstream s;
  s << *const1;
  EXPECT_EQ(s.str(), "[3.1415] pi");
}

TEST(var_declt_ostream, normal_test)
{
  auto var1 = std::make_unique<var_declt>(
    std::make_unique<symbolt>("a1"),
    std::make_unique<intervalt>(
      std::make_unique<numbert>("-0.1254"),
      std::make_unique<numbert>("1.43e2")));

  stringstream s;
  s << *var1;
  EXPECT_EQ(s.str(), "[-0.1254, 1.43e2] a1");
}

TEST(dist_declt_ostream, normal_test)
{
  auto rv1 = std::make_unique<dist_declt>(
    std::make_unique<symbolt>("p_11"),
    std::make_unique<uniform_distt>(
      std::make_unique<numbert>("-0.1254"),
      std::make_unique<numbert>("1.43e2")));

  stringstream s;
  s << *rv1;
  EXPECT_EQ(s.str(), "dist_uniform(-0.1254, 1.43e2) p_11");
}
