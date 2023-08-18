#pragma once

#include "node.hpp"

#include <unordered_map>

typedef uint16_t TYPEINDEX;
typedef std::string TYPENAME;

extern std::unordered_map<TYPEINDEX, TYPENAME> TypeContext;

class Type : public Node {
protected:
  TYPEINDEX _type_index;
public:
  Type() {}
  Type(TYPEINDEX _type_index) : _type_index(_type_index) {}
  const TYPEINDEX type_index() { return _type_index; }
  TYPENAME type_name() { return TypeContext[_type_index]; }
  virtual bool isSameAs(Type*) = 0;
  REGISTER_NODE_VIRTUAL_FUNC;
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
  };
public:
  IntegerType() {
    _type_index = 1;
  }
};

