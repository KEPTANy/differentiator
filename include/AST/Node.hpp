#pragma once

#include "AST/Visitors/NodeVisitor.hpp"
#include "Token.hpp"

class Node {
public:
  Node(TokenType type);

  virtual ~Node() = default;
  virtual void accept(NodeVisitor &v) const = 0;

  TokenType get_type() const;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  TokenType type{};
};
