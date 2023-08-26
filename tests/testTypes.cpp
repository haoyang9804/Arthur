#include <gtest/gtest.h>

#include "../include/type.hpp"
#include "../include/visitor.hpp"
#include "../include/config.hpp"

#include <iostream>

Visitor* v = new PrintVisitor();


TEST(testTypes, testIntegerType) {
  
  Type* t1 = new IntegerType(0);
  ASSERT_EQ(t1->accept(v), "int");
  ASSERT_STREQ(t1->type_name().c_str(), "Integer");
  ASSERT_TRUE(t1->isValueType());
  Type* t1_ = new IntegerType(0);
  ASSERT_TRUE(t1->isSameAs(t1_));
  ASSERT_TRUE(t1_->isSameAs(t1));

  Type* t2 = new IntegerType(1);
  ASSERT_EQ(t2->accept(v), "uint");
  ASSERT_STREQ(t2->type_name().c_str(), "Integer");
  ASSERT_TRUE(t2->isValueType());
  Type* t2_ = new IntegerType(1);
  ASSERT_TRUE(t2->isSameAs(t2_));
  ASSERT_TRUE(t2_->isSameAs(t2));

  ASSERT_FALSE(t1->isSameAs(t2));
  ASSERT_FALSE(t2->isSameAs(t1));
}

TEST(testTypes, testBooleanType){
  Type* t1 = new BooleanType();
  ASSERT_EQ(t1->accept(v), "bool");
  ASSERT_STREQ(t1->type_name().c_str(), "Bool");
  ASSERT_TRUE(t1->isValueType());
  Type* t2 = new BooleanType();
  ASSERT_TRUE(t1->isSameAs(t2));
  ASSERT_TRUE(t2->isSameAs(t1));
}

TEST(testTypes, testAddressType){
  
  Type* t1 = new AddressType(0);
  ASSERT_EQ(t1->accept(v), "address");
  ASSERT_STREQ(t1->type_name().c_str(), "Address");
  ASSERT_TRUE(t1->isValueType());
  
  Type* t2 = new AddressType(1);
  ASSERT_EQ(t2->accept(v), "address payable");
  ASSERT_STREQ(t2->type_name().c_str(), "Address");
  ASSERT_TRUE(t2->isValueType());

  Type* t1_ = new AddressType(0), * t2_ = new AddressType(1);
  ASSERT_TRUE(t1_->isSameAs(t1) && t1->isSameAs(t1_));
  ASSERT_TRUE(t2_->isSameAs(t2) && t2->isSameAs(t2_));
  ASSERT_FALSE(t1->isSameAs(t2) || t2->isSameAs(t1));  
}

TEST(testTypes, testBytesType) {
  Type* t1 = new BytesType(1);
  ASSERT_EQ(t1->accept(v), "bytes1");
  ASSERT_STREQ(t1->type_name().c_str(), "Bytes");
  ASSERT_TRUE(t1->isValueType());
  ASSERT_THROW(new BytesType(33), std::logic_error); 
  Type* t2 = new BytesType(2);
  ASSERT_FALSE(t1->isSameAs(t2) || t2->isSameAs(t1));
}

TEST(testTypes, testStringType) {
  Type* t1 = new StringType();
  ASSERT_EQ(t1->accept(v), "string");
  ASSERT_STREQ(t1->type_name().c_str(), "String");
  ASSERT_TRUE(t1->isValueType());
  Type* t2 = new StringType();
  ASSERT_TRUE(t1->isSameAs(t2) && t2->isSameAs(t1));
}

// TEST(testTypes, testFunctionType) {
//   Type* t1 = new FunctionType(0, 1)
// }

TEST(testTypes, testValueType_2) {

  Type* t1 = new EnumType(randomlyPickAWord());
  std::cout << t1->type_index() << std::endl;
  ASSERT_EQ(t1->type_index(), 257);
  ASSERT_TRUE(t1->isValueType());

  Type* t2 = new TypeAliasType(5, randomlyPickAWord());
  ASSERT_EQ(t2->type_index(), 258);
  ASSERT_TRUE(t2->isValueType());
}