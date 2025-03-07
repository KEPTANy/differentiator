#pragma once

#include <string>

#include "AST/Visitors/NodeVisitor.hpp"

class NodeVisitorStringifier : public NodeVisitor {
public:
  ~NodeVisitorStringifier() override = default;

  std::string result() const;

  void visit(const NodeBinary &node) override;
  void visit(const NodeFunction &node) override;
  void visit(const NodeUnary &node) override;
  void visit(const NodeValue &node) override;
  void visit(const NodeVariable &node) override;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::string res{};
  int precedence{};
};
