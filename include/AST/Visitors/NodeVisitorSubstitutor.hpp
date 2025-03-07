#pragma once

#include "AST/Node.hpp"
#include "AST/Visitors/NodeVisitorReplicator.hpp"

class NodeVisitorSubstitutor : public NodeVisitorReplicator {
public:
  NodeVisitorSubstitutor(std::string var_name, const Node &expr);
  ~NodeVisitorSubstitutor() override = default;

  void visit(const NodeVariable &node) override;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::string var_name;
  const Node &expr;
};
