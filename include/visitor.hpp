#pragma once

#include "node.hpp"
#include "utils.hpp"

struct Visitor {
  virtual std::string visit(Node* t) = 0;
};

struct PrintVisitor : Visitor {
  std::string visit(Node* t) {
    return t->toString();
  }
};