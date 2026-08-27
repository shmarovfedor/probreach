//
// Created by fedor on 23/01/17.
//

#include <gtest/gtest.h>
#include "node.h"
#include <cmath>

using namespace std;
using namespace pdrh;

/**
 * Testing node copying
 */
TEST(node_copy_and_delete, normal_test)
{
  node *const1 = new node("1");
  node *const2 = new node("3");
  node *x = new node("x");
  node *src = new node("/", {new node("+", {x, const1}), const2});

  // deep copy
  node *copy = src->copy();
  EXPECT_EQ(src->to_infix(), copy->to_infix());

  // using copy constructor
  node *copy2(src);
  EXPECT_EQ(src->to_infix(), copy2->to_infix());
  EXPECT_EQ(copy->to_infix(), copy2->to_infix());
  
  src->operands[0]->value = "-";
  
  EXPECT_EQ(src->to_infix(), copy2->to_infix());
  EXPECT_NE(copy->to_infix(), copy2->to_infix());

  cerr << "SRC: " << src->to_infix() << "\n";
  cerr << "COPY (copy): " << copy->to_infix() << "\n";
  cerr << "COPY2 (constructor): " << copy2->to_infix() << "\n";

  delete src;
  delete copy;
}
