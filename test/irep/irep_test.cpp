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
