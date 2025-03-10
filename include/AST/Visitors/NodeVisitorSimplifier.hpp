#pragma once

#include "AST/Visitors/NodeVisitorReplicator.hpp"

class NodeVisitorSimplifier : public NodeVisitorReplicator {
public:
  ~NodeVisitorSimplifier() override = default;

  void visit(const NodeBinary &node) override;
  void visit(const NodeFunction &node) override;
  void visit(const NodeUnary &node) override;
};
