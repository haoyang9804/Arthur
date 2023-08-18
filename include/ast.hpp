#pragma once

#include <vector>


class OperatorLiteral;

class BoolLiteral {
public:
  type type_() { return type.BOOL; }
  
};

class IntegerLiteral {

};



/*
According to https://docs.soliditylang.org/en/v0.8.21/types.html#value-types as of 17/Aug/2023,
Fixed point numbers are not fully supported by Solidity yet. They can be declared, but cannot be assigned to or from.
So FloatType is a TODO here.
*/
struct FloatLiteral;
