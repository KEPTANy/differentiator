#include "AST/Visitors/NodeVisitorSimplifier.hpp"

#include <cmath>
#include <memory>

#include "AST/NodeBinary.hpp"
#include "AST/NodeFunction.hpp"
#include "AST/NodeUnary.hpp"
#include "AST/NodeValue.hpp"

void NodeVisitorSimplifier::visit(const NodeBinary &node) {
  node.get_left_expr().accept(*this);
  auto l{result()};

  node.get_right_expr().accept(*this);
  auto r{result()};

  // if both arguments are numbers, compute value directly
  if (dynamic_cast<const NodeValue *>(l.get()) &&
      dynamic_cast<const NodeValue *>(r.get())) {
    auto val_l{dynamic_cast<const NodeValue *>(l.get())->get_val()};
    auto val_r{dynamic_cast<const NodeValue *>(r.get())->get_val()};
    if (node.get_type() == TokenType::PLUS) {
      res = std::make_unique<NodeValue>(val_l + val_r);
    } else if (node.get_type() == TokenType::MINUS) {
      res = std::make_unique<NodeValue>(val_l - val_r);
    } else if (node.get_type() == TokenType::STAR) {
      res = std::make_unique<NodeValue>(val_l * val_r);
    } else if (node.get_type() == TokenType::SLASH) {
      res = std::make_unique<NodeValue>(val_l / val_r);
    } else if (node.get_type() == TokenType::POW) {
      res = std::make_unique<NodeValue>(std::pow(val_l, val_r));
    } else {
      throw std::invalid_argument("Unknown binary operation");
    }
    return;
  }

  res =
      std::make_unique<NodeBinary>(std::move(l), node.get_type(), std::move(r));
}

void NodeVisitorSimplifier::visit(const NodeFunction &node) {
  node.get_expr().accept(*this);
  auto arg{result()};

  // exp(ln(x)) -> x
  // ln(exp(x)) -> x
  if (auto raw_arg{dynamic_cast<const NodeFunction *>(arg.get())}) {
    if ((node.get_name() == "exp" && raw_arg->get_name() == "ln") ||
        (node.get_name() == "ln" && raw_arg->get_name() == "exp")) {
      raw_arg->get_expr().accept(*this);
      return;
    }
  }

  // if an argument is a number, compute value directly
  if (auto raw_arg{dynamic_cast<const NodeValue *>(arg.get())}) {
    if (node.get_name() == "sin") {
      res = std::make_unique<NodeValue>(std::sin(raw_arg->get_val()));
    } else if (node.get_name() == "cos") {
      res = std::make_unique<NodeValue>(std::cos(raw_arg->get_val()));
    } else if (node.get_name() == "exp") {
      res = std::make_unique<NodeValue>(std::exp(raw_arg->get_val()));
    } else if (node.get_name() == "ln") {
      res = std::make_unique<NodeValue>(std::log(raw_arg->get_val()));
    } else {
      throw std::invalid_argument("Unknown function");
    }
    return;
  }

  res = std::make_unique<NodeFunction>(node.get_name(), std::move(arg));
}

void NodeVisitorSimplifier::visit(const NodeUnary &node) {
  node.get_expr().accept(*this);
  auto arg{result()};

  // if an argument is a number, compute value directly
  if (auto raw_arg{dynamic_cast<const NodeValue *>(arg.get())}) {
    if (node.get_type() == TokenType::MINUS) {
      res = std::make_unique<NodeValue>(std::sin(raw_arg->get_val()));
    } else {
      throw std::invalid_argument("Unknown unary operation");
    }
    return;
  }

  res = std::make_unique<NodeUnary>(TokenType::MINUS, std::move(arg));
}
