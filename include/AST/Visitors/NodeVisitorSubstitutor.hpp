#pragma once

#include <memory>

#include "AST/Node.hpp"
#include "AST/Visitors/NodeVisitor.hpp"

class NodeVisitorSubstitutor : public NodeVisitor {
public:
  NodeVisitorSubstitutor(std::string var_name, const Node &expr);
  ~NodeVisitorSubstitutor() override = default;

  std::unique_ptr<Node> result();

  void visit(const NodeBinary &node) override;
  void visit(const NodeFunction &node) override;
  void visit(const NodeUnary &node) override;
  void visit(const NodeValue &node) override;
  void visit(const NodeVariable &node) override;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::string var_name;
  const Node &expr;
  std::unique_ptr<Node> res{};
};
