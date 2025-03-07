#pragma once

#include <memory>

#include "AST/Visitors/NodeVisitor.hpp"

class NodeVisitorReplicator : public NodeVisitor {
public:
  ~NodeVisitorReplicator() override = default;

  std::unique_ptr<Node> result();

  void visit(const NodeBinary &node) override;
  void visit(const NodeFunction &node) override;
  void visit(const NodeUnary &node) override;
  void visit(const NodeValue &node) override;
  void visit(const NodeVariable &node) override;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
protected:
#endif
  std::unique_ptr<Node> res{};
};
