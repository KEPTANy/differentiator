#pragma once

#include <memory>

#include "AST/Node.hpp"
#include "Token.hpp"

class NodeBinary : public Node {
public:
  NodeBinary(std::unique_ptr<Node> l, TokenType type, std::unique_ptr<Node> r);

  void accept(NodeVisitor &v) const override;

  const Node &get_left_expr() const;
  const Node &get_right_expr() const;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::unique_ptr<Node> left{};
  std::unique_ptr<Node> right{};
};
