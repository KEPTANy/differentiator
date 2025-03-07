#include "AST/Visitors/NodeVisitorReplicator.hpp"

#include "AST/Node.hpp"
#include "AST/NodeBinary.hpp"
#include "AST/NodeFunction.hpp"
#include "AST/NodeUnary.hpp"
#include "AST/NodeValue.hpp"
#include "AST/NodeVariable.hpp"

std::unique_ptr<Node> NodeVisitorReplicator::result() { return std::move(res); }

void NodeVisitorReplicator::visit(const NodeBinary &node) {
  node.get_left_expr().accept(*this);
  auto l{result()};
  node.get_right_expr().accept(*this);
  auto r{result()};

  res =
      std::make_unique<NodeBinary>(std::move(l), node.get_type(), std::move(r));
}

void NodeVisitorReplicator::visit(const NodeFunction &node) {
  node.get_expr().accept(*this);
  auto e{result()};

  res = std::make_unique<NodeFunction>(node.get_name(), std::move(e));
}

void NodeVisitorReplicator::visit(const NodeUnary &node) {
  node.get_expr().accept(*this);
  auto e{result()};

  res = std::make_unique<NodeUnary>(node.get_type(), std::move(e));
}

void NodeVisitorReplicator::visit(const NodeValue &node) {
  res = std::make_unique<NodeValue>(node.get_val());
}

void NodeVisitorReplicator::visit(const NodeVariable &node) {
  res = std::make_unique<NodeVariable>(node.get_name());
}
