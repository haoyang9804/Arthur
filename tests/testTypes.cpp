#include <gtest/gtest.h>

#include "../include/type.hpp"
#include "../include/visitor.hpp"

TEST(testTypes, visitType) {
  Visitor* v = new PrintVisitor();
  Type* t1 = new IntegerType();
  ASSERT_EQ(t1->accept(v), "int");
  Type* t2 = new IntegerType(1);
  ASSERT_EQ(t2->accept(v), "uint");
  Type* t3 = new BooleanType();
  ASSERT_EQ(t3->accept(v), "bool");
}