#pragma once

#include <string>

struct Visitor;

#define REGISTER_NODE_VIRTUAL_FUNC \
  virtual std::string accept(Visitor* v) = 0; \
  virtual std::string str() = 0;

struct Node {
  /**
   * @brief accept a visitor, when the node is visited,
   * the visitor should first write the basic info of this
   * node into the log and perform core tasks, such as
   * generating a code fragment from the AST node.
   * 
   * @param v 
   * @return std::string 
   */
  virtual std::string accept(Visitor* v) = 0;
  virtual std::string str() = 0;
};