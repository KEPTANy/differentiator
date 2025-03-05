#pragma once

#include <memory>

#include "AST/Node.hpp"
#include "Token.hpp"

class NodeUnary : public Node {
public:
  NodeUnary(TokenType type, std::unique_ptr<Node> expr);

  void accept(NodeVisitor &v) const override;

  const Node &get_expr() const;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::unique_ptr<Node> expr{};
};
