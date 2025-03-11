#pragma once

#include <memory>
#include <string>

#include "AST/Visitors/NodeVisitor.hpp"

class NodeVisitorDifferentiator : public NodeVisitor {
public:
  NodeVisitorDifferentiator(std::string wrt);
  ~NodeVisitorDifferentiator() override = default;

  std::unique_ptr<Node> result();

  void visit(const NodeBinary &node) override;
  void visit(const NodeFunction &node) override;
  void visit(const NodeUnary &node) override;
  void visit(const NodeValue &node) override;
  void visit(const NodeVariable &node) override;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::unique_ptr<Node> res{};
  std::string wrt{};
};
