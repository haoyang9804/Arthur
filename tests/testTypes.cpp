#include <gtest/gtest.h>

#include "../include/type.hpp"
#include "../include/visitor.hpp"

TEST(testTypes, visitType) {
  Visitor* v = new PrintVisitor();
  Type* t1 = new IntegerType(0);
  ASSERT_EQ(t1->accept(v), "int");
  Type* t2 = new IntegerType(1);
  ASSERT_EQ(t2->accept(v), "uint");
  Type* t3 = new BooleanType();
  ASSERT_EQ(t3->accept(v), "bool");
  Type* t4 = new AddressType(0);
  ASSERT_EQ(t4->accept(v), "address");
  Type* t5 = new AddressType(1);
  ASSERT_EQ(t5->accept(v), "address payable");
  Type* t6 = new BytesType(1);
  ASSERT_EQ(t6->accept(v), "bytes1");
  ASSERT_THROW(
    new BytesType(33),
  std::logic_error); 
}