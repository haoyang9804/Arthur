#pragma once

#include <assert.h>

#include <unordered_map>

#include "node.hpp"
#include "visitor.hpp"

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
  virtual bool isValueType() { return false; }
  virtual bool isReferenceType() { return false; }
  // bytes (without numerical suffix) and string are two dynamically-sized type
  virtual bool isDynamicSizeType() { return false; }
};

class ValueType : public Type {
 public:
  bool isValueType() final { return true; }
};

class IntegerType : public ValueType {
 private:
  struct infoBitField {
    unsigned isUnsigned : 1;
    /*
      TODO:

      There should be other bits, such as `size` to represent
      the size suffix, such as `int8`.
    */
    infoBitField(unsigned _isUnsigned) : isUnsigned(_isUnsigned) {}

    bool operator==(const infoBitField&& b) const {
      return isUnsigned == b.isUnsigned;
    }
  };
  infoBitField bit{1};

 public:
  IntegerType() =
      delete;  // you must specify the attributes, such as isUnsigned
  IntegerType(unsigned isUnsigned) {
    _type_index = 1;
    bit.isUnsigned = isUnsigned;
  }
  /*
    TODO:

    After updating infoBitField, this function should also be updated.
  */

  infoBitField info() { return bit; }

  bool isSameAs(Type* t) final {
    if (this->_type_index != t->type_index()) return false;
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

  std::string accept(Visitor* v) final { return v->visit(this); }
};

class BooleanType : public ValueType {
 public:
  BooleanType() { _type_index = 2; }

  std::string toString() final { return "bool"; }

  std::string accept(Visitor* v) final { return v->visit(this); }
};

// Fixed Point Numbers are under development.
// We temporarily leave it void here.
class FloatType;  // also a ValueType

class AddressType : public ValueType {
 private:
  struct infoBitField {
    unsigned isPayable : 1;
    bool operator==(const infoBitField&& b) const {
      return this->isPayable == b.isPayable;
    }
  };
  infoBitField bit{0};

 public:
  AddressType() = delete;  // you must specify the `isPayable` attribute
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

  infoBitField info() { return bit; }

  std::string accept(Visitor* v) final { return v->visit(this); }

  bool isSameAs(Type* t) final {
    if (this->_type_index != t->type_index()) return false;
    AddressType* addresst = dynamic_cast<AddressType*>(t);
    assert(addresst != nullptr);
    return this->info() == addresst->info();
  }
};

class BytesType : public ValueType {
 private:
  uint16_t _suffix;

 public:
  uint16_t suffix() { return _suffix; }
  BytesType() = delete;  // you must specify the suffix
  BytesType(uint16_t suffix_) {
    ASSERT_LOGIC(
        suffix_ <= 32 && suffix_ >= 1,
        "suffix " + std::to_string(suffix_) + " of bytes is out-of-range");
    _suffix = suffix_;
    _type_index = 4;
  }
  bool isSameAs(Type* t) final {
    if (this->_type_index != t->type_index()) return false;
    BytesType* bytest = dynamic_cast<BytesType*>(t);
    assert(bytest != nullptr);
    return this->suffix() == bytest->suffix();
  }
  std::string toString() final { return "bytes" + std::to_string(_suffix); }
  std::string accept(Visitor* v) final { return v->visit(this); }
};

class StringType : public ValueType {
 public:
  StringType() { _type_index = 5; }
  bool isSameAs(Type* t) final { return this->_type_index == t->type_index(); }
  std::string toString() final { return "string"; }
  std::string accept(Visitor* v) final { return v->visit(this); }
  bool isDynamicSizeType() final { return true; }
};

class FunctionType : public ValueType {
 private:
  struct infoBitField {
    // 000 - three bits
    // first bit  = 1 -> public
    // second bit = 1 -> internal
    // third bit  = 2 -> external
    unsigned attr : 3;
    unsigned scope : 2;
    // 000 - three bits
    // first bit  = 1 -> pure
    // second bit = 1 -> view
    // third bit  = 2 -> payable
    unsigned attr : 3;

    bool operator==(const infoBitField&& b) const {
      ASSERT_LOGIC(
          b.attr <= 7 && attr <= 7,
          "Invalid attr value here, b.attr = " + std::to_string(b.attr) +
              " and attr = " + std::to_string(attr));
      ASSERT_LOGIC(
          b.scope <= 7 && scope <= 7,
          "Invalid scope value here, b.scope = " + std::to_string(b.scope) +
              " and scope = " + std::to_string(scope));
      return scope == b.scope && attr == b.attr;
    }

    bool operator!=(const infoBitField&& b) const {
      return !(scope == b.scope && attr == b.attr);
    }
  };

  infoBitField b{0, 0};

  std::vector<Type*> _retType;
  std::vector<Type*> _paramType;

 public:
  FunctionType() = delete;
  FunctionType(unsigned scope, unsigned attr,
               std::vector<Type*> const&& retType_ = {},
               std::vector<Type*> const&& paramTypes_ = {}) {
    ASSERT_LOGIC(attr <= 7,
                 "Invalid attr value here, attr = " + std::to_string(attr));
    ASSERT_LOGIC(scope <= 7,
                 "Invalid scope value here, scope = " + std::to_string(scope));
    #ifndef ERROR
    assert(typeCheck());
    #endif
    b.scope = scope;
    b.attr = attr;
    _retType = retType_;
    _paramType = paramTypes_;
    _type_index = 6;
  }

  infoBitField info() { return b; }

  std::vector<Type*> retType() { return _retType; }

  std::vector<Type*> paramType() { return _paramType; }

  bool isPure() { return b.attr & 0x01; }

  bool isView() { return b.attr & 0x02; }

  bool isPayable() { return b.attr & 0x04; }

  bool isInternal() { return b.scope & 0x02; }

  bool isExternal() { return b.scope & 0x04; }

  bool isPublic() { return b.scope & 0x01; }

  #ifndef ERROR
  bool typeCheck() {
    return !(b.attr != 0x01 && b.attr != 0x02 && b.attr != 0x04 || b.scope != 0x01 && b.scope != 0x02 && b.scope != 0x04);
  }
  #endif

  bool isSameAs(Type* t) final {
    if (this->_type_index != t->type_index()) return false;
    FunctionType* ftype = dynamic_cast<FunctionType*>(t);
    assert(ftype != nullptr);
    if (b != ftype->info()) return false;
    auto t_retType = ftype->retType();
    auto t_paramType = ftype->retType();
    size_t ret_size = _retType.size(), t_ret_size = t_retType.size();
    if (ret_size != t_ret_size) return false;
    for (size_t i = 0; i < ret_size; i++) {
      if (!(_retType[i]->isSameAs(t_retType[i]))) return false;
    }
    size_t param_size = _paramType.size(), t_param_size = t_paramType.size();
    if (param_size != t_param_size) return false;
    for (size_t i = 0; i < param_size; i++) {
      if (!(_paramType[i]->isSameAs(t_paramType[i]))) return false;
    }
    return true;
  }

  std::string toString() final {
    std::string ret = "function(";
    size_t param_size = _paramType.size();
    for (size_t i = 0; i < param_size; i++) {
      ret += _paramType[i]->toString();
      if (i != param_size - 1) ret += ", ";
    }
    ret += ") ";
    if (isInternal()) ret += "internal";
    if (isPublic()) ret += "public";
    else if (isExternal()) ret += "external";
    if (isPayable()) ret += "payable";
    e
    size_t ret_size = _retType.size();
    // if (! ret_size)
    return ret;
  }

  // TODO
  std::string toString4func() { return ""; }

  std::string accept(Visitor* v) final { return v->visit(this); }
};

class UserType : public ValueType {
 protected:
  std::string _name;

 public:
  UserType() { _type_index = user_defined_type_index++; }
  std::string name() { return _name; }
};

class EnumType : public UserType {
 public:
  EnumType() = delete;  // you must specify the name;
  EnumType(std::string name_) {
    ASSERT_LOGIC(name_.size() > 0, "EnumType should have a non-empty name");
    _name = name_;
  }
  bool isSameAs(Type* t) final {
    if (this->_type_index != t->type_index()) return false;
    EnumType* enumt = dynamic_cast<EnumType*>(t);
    return enumt->name() == this->_name;
  }
  std::string toString() final { return _name; }
  std::string accept(Visitor* v) final { return v->visit(this); }
};

class TypeAliasType : public UserType {
 private:
  uint16_t _alias_type_index;

 public:
  uint16_t alias_type_index() { return _alias_type_index; }
  TypeAliasType() = delete;
  TypeAliasType(uint16_t alias_type_index_, std::string name_) {
    // TODO: rewrite this ASSERT
    ASSERT_LOGIC(alias_type_index_ >= 257 &&
                         alias_type_index_ < user_defined_type_index ||
                     alias_type_index_ <= 6,
                 "alias_type_index_ is " + std::to_string(alias_type_index_) +
                     ", which is out-of-scope: " + std::to_string(6));
    _alias_type_index = alias_type_index_;
    _name = name_;
  }
  bool isSameAs(Type* t) final { return this->_type_index == t->type_index(); }
  std::string toString() final { return _name; }
  std::string accept(Visitor* v) final { return v->visit(this); }
};