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
  node *res = new node("/", {new node("+", {x, const1}), const2});

  node *copy = copy_node(res);
  EXPECT_TRUE(res->value == copy->value);
  EXPECT_TRUE(res->operands.size() == copy->operands.size());
  for (int i = 0; i < res->operands.size(); i++)
    EXPECT_TRUE(res->operands[i]->value == copy->operands[i]->value);

  node other(*copy);
  other.value = "*";
  other.operands[0]->value = "-";
  
  cerr << "res(infix) : " << res->to_infix() << "\n";
  cerr << "res(prefix) : " << res->to_prefix() << "\n";
  cerr << "res(stream) : " << *res << "\n";
  cerr << "copy(infix) : " << copy->to_infix() << "\n";
  cerr << "copy(prefix) : " << copy->to_prefix() << "\n";
  cerr << "copy(stream) : " << *copy << "\n";
  cerr << "other(infix) : " << other.to_infix() << "\n";
  cerr << "other(prefix) : " << other.to_prefix() << "\n";
  cerr << "other(stream) : " << other << "\n";


}
