#include <gtest/gtest.h>

#include "../include/type.hpp"
#include "../include/visitor.hpp"
#include "../include/config.hpp"

#include <iostream>

TEST(testTypes, visitType) {
  Visitor* v = new PrintVisitor();
  Type* t1 = new IntegerType(0);
  ASSERT_EQ(t1->accept(v), "int");
  ASSERT_STREQ(t1->type_name().c_str(), "Integer");
  Type* t2 = new IntegerType(1);
  ASSERT_EQ(t2->accept(v), "uint");
  ASSERT_STREQ(t2->type_name().c_str(), "Integer");
  Type* t3 = new BooleanType();
  ASSERT_EQ(t3->accept(v), "bool");
  ASSERT_STREQ(t3->type_name().c_str(), "Bool");
  Type* t4 = new AddressType(0);
  ASSERT_EQ(t4->accept(v), "address");
  ASSERT_STREQ(t4->type_name().c_str(), "Address");
  Type* t5 = new AddressType(1);
  ASSERT_EQ(t5->accept(v), "address payable");
  ASSERT_STREQ(t5->type_name().c_str(), "Address");
  Type* t6 = new BytesType(1);
  ASSERT_EQ(t6->accept(v), "bytes1");
  ASSERT_STREQ(t6->type_name().c_str(), "Bytes");
  ASSERT_THROW(
    new BytesType(33),
  std::logic_error); 
  Type* t7 = new StringType();
  ASSERT_EQ(t7->accept(v), "string");
  ASSERT_STREQ(t7->type_name().c_str(), "String");
  Type* t8 = new EnumType(randomlyPickAWord());
  std::cout << t8->type_index() << std::endl;
  ASSERT_EQ(t8->type_index(), 257);
  std::cout << t8->accept(v) << std::endl;
}