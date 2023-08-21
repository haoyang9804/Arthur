#pragma once

#include <string>

struct Visitor;


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
  virtual std::string toString() = 0;
};