#pragma once

#include "node.hpp"
#include "visitor.hpp"

#include <unordered_map>
#include <assert.h>

extern std::unordered_map<uint16_t, std::string> TypeContext;
extern uint16_t user_defined_type_index;

class Type : public Node {
protected:
  uint16_t _type_index;
public:
  Type() {}
  Type(uint16_t _type_index) : _type_index(_type_index) {}
  const uint16_t type_index() { return _type_index; }
  std::string type_name() { return TypeContext[_type_index]; }
  virtual bool isSameAs(Type* t) {
    return this->_type_index == t->type_index();
  }
  virtual std::string toString() = 0;
  virtual std::string accept(Visitor* v) = 0;  
};

class IntegerType : public Type {
private:
  struct infoBitField {
    unsigned isUnsigned : 1;
    /*
      TODO:
      
      There should be other bits, such as `size` to represent
      the size suffix, such as `int8`. 
    */
    infoBitField(unsigned _isUnsigned): \
      isUnsigned(_isUnsigned)
    {}

    bool operator== (const infoBitField&& b) const {
      return isUnsigned == b.isUnsigned;
    }
  };
  infoBitField bit{1};
public:
  IntegerType() = delete; // you must specify the attributes, such as isUnsigned
  IntegerType(unsigned isUnsigned) {
    _type_index = 1;
    bit.isUnsigned = isUnsigned;
  }
  /*
    TODO:

    After updating infoBitField, this function should also be updated.
  */

  infoBitField info() {
    return bit;
  }

  bool isSameAs(Type* t) final {
    if (this->_type_index != t->type_index())
      return false;
    IntegerType* intt = dynamic_cast<IntegerType*>(t);
    assert(intt != nullptr);
    return this->info() == intt->info();
  }

  std::string toString() final {
    std::string res = "";
    if (bit.isUnsigned) {
      res += "u";
    }
    res += "int";
    return res;
  }

  std::string accept(Visitor* v) final {
    return v->visit(this);
  }
  
};

class BooleanType : public Type {
public:
  BooleanType() {
    _type_index = 2;
  }

  std::string toString() final {
    return "bool";
  }

  std::string accept(Visitor* v) final {
    return v->visit(this);
  }
};


// Fixed Point Numbers are under development.
// We temporarily leave it void here.
class FloatType;

class AddressType : public Type {
private:
  struct infoBitField {
    unsigned isPayable : 1;
    bool operator== (const infoBitField&& b) const {
      return this->isPayable == b.isPayable;
    }
  };
  infoBitField bit{0};

public:
  AddressType() = delete; // you must specify the `isPayable` attribute
  AddressType(unsigned isPayable) {
    _type_index = 3;
    bit.isPayable = isPayable;
  }

  std::string toString() final {
    std::string res = "address";
    if (bit.isPayable) {
      res += " payable";
    }
    return res;
  }

  infoBitField info() {
    return bit;
  }

  std::string accept(Visitor* v) final {
    return v->visit(this);
  }

  bool isSameAs(Type* t) final {
    if (this->_type_index != t->type_index())
      return false;
    AddressType* addresst = dynamic_cast<AddressType*>(t);
    assert(addresst != nullptr);
    return this->info() == addresst->info();
  }
};

class BytesType : public Type {
private:
  uint16_t _suffix;
public:
  uint16_t suffix() { return _suffix; }
  BytesType() = delete; // you must specify the suffix
  BytesType(uint16_t suffix_) {
    ASSERT_LOGIC(suffix_ <= 32 && suffix_ >= 1, "suffix " + std::to_string(suffix_) + " of bytes is out-of-range");
    _suffix = suffix_; 
    _type_index = 4; 
  }
  bool isSameAs(Type* t) final {
    if (this->_type_index != t->type_index())
      return false;
    BytesType* bytest = dynamic_cast<BytesType*>(t);
    assert(bytest != nullptr);
    return this->suffix() == bytest->suffix();
  }
  std::string toString() final {
    return "bytes" + std::to_string(_suffix);
  }
  std::string accept(Visitor* v) final {
    return v->visit(this);
  } 
};

class StringType : public Type {
public:
  StringType() {
    _type_index = 5;
  }
  bool isSameAs(Type* t) final {
    return this->_type_index == t->type_index();
  }
  std::string toString() final {
    return "string";
  }
  std::string accept(Visitor* v) final {
    return v->visit(this);
  }  
};

class UserType : public Type {
protected:
  std::string _name;
public: 
  UserType() { _type_index = user_defined_type_index++; }
  std::string name() { return _name; }
};

class EnumType : public UserType {
public:
  EnumType () = delete; // you must specify the name;
  EnumType (std::string name_) {
    ASSERT_LOGIC(name_.size() > 0, "EnumType should have a non-empty name");
    _name = name_;
  }
  bool isSameAs(Type* t) final {
    if (this->_type_index != t->type_index())
      return false;
    EnumType* enumt = dynamic_cast<EnumType*>(t);
    return enumt->name() == this->_name;
  }
  std::string toString() final {
    return _name;
  }
  std::string accept(Visitor* v) final {
    return v->visit(this);
  }
};

