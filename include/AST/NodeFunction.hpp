#pragma once

#include <memory>
#include <string>

#include "AST/Node.hpp"

class NodeFunction : public Node {
public:
  NodeFunction(std::string name, std::unique_ptr<Node> expr);

  void accept(NodeVisitor &v) const override;

  const std::string &get_name() const;
  const Node &get_expr() const;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::string name{};
  std::unique_ptr<Node> expr{};
};
