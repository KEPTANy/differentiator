#include "AST/Visitors/NodeVisitorSubstitutor.hpp"

#include <memory>

#include "AST/Node.hpp"
#include "AST/NodeVariable.hpp"
#include "AST/Visitors/NodeVisitorReplicator.hpp"

NodeVisitorSubstitutor::NodeVisitorSubstitutor(std::string var_name,
                                               const Node &expr)
    : var_name{var_name}, expr{expr} {}

void NodeVisitorSubstitutor::visit(const NodeVariable &node) {
  if (node.get_name() == var_name) {
    NodeVisitorReplicator vis{};
    expr.accept(vis);
    res = vis.result();
  } else {
    res = std::make_unique<NodeVariable>(node.get_name());
  }
}
