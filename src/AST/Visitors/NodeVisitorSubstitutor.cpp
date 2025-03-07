#include "AST/Visitors/NodeVisitorSubstitutor.hpp"

#include "AST/Node.hpp"
#include "AST/NodeBinary.hpp"
#include "AST/NodeFunction.hpp"
#include "AST/NodeUnary.hpp"
#include "AST/NodeValue.hpp"
#include "AST/NodeVariable.hpp"
#include "AST/Visitors/NodeVisitorReplicator.hpp"

NodeVisitorSubstitutor::NodeVisitorSubstitutor(std::string var_name,
                                               const Node &expr)
    : var_name{var_name}, expr{expr} {}

std::unique_ptr<Node> NodeVisitorSubstitutor::result() {
  return std::move(res);
}

void NodeVisitorSubstitutor::visit(const NodeBinary &node) {
  node.get_left_expr().accept(*this);
  auto l{result()};
  node.get_right_expr().accept(*this);
  auto r{result()};

  res =
      std::make_unique<NodeBinary>(std::move(l), node.get_type(), std::move(r));
}

void NodeVisitorSubstitutor::visit(const NodeFunction &node) {
  node.get_expr().accept(*this);
  auto e{result()};

  res = std::make_unique<NodeFunction>(node.get_name(), std::move(e));
}

void NodeVisitorSubstitutor::visit(const NodeUnary &node) {
  node.get_expr().accept(*this);
  auto e{result()};

  res = std::make_unique<NodeUnary>(node.get_type(), std::move(e));
}

void NodeVisitorSubstitutor::visit(const NodeValue &node) {
  res = std::make_unique<NodeValue>(node.get_val());
}

void NodeVisitorSubstitutor::visit(const NodeVariable &node) {
  if (node.get_name() == var_name) {
    NodeVisitorReplicator vis{};
    expr.accept(vis);
    res = vis.result();
  } else {
    res = std::make_unique<NodeVariable>(node.get_name());
  }
}
