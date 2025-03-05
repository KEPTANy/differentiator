#include "AST/Visitors/NodeVisitorStringifier.hpp"

#include "AST/Node.hpp"
#include "AST/NodeBinary.hpp"
#include "AST/NodeFunction.hpp"
#include "AST/NodeUnary.hpp"
#include "AST/NodeValue.hpp"
#include "AST/NodeVariable.hpp"

std::string NodeVisitorStringifier::result() const { return res; }

void NodeVisitorStringifier::visit(const NodeBinary &node) {
  node.get_left_expr().accept(*this);
  auto l{result()};
  node.get_right_expr().accept(*this);
  auto r{result()};

  auto op{node.get_type()};
  switch (op) {
  case TokenType::PLUS:
    res = '(' + l + ")+(" + r + ')';
    break;
  case TokenType::MINUS:
    res = '(' + l + ")-(" + r + ')';
    break;
  case TokenType::STAR:
    res = '(' + l + ")*(" + r + ')';
    break;
  case TokenType::SLASH:
    res = '(' + l + ")/(" + r + ')';
    break;
  default:
    throw "Unknown binary operation";
  }
}

void NodeVisitorStringifier::visit(const NodeFunction &node) {
  node.get_expr().accept(*this);
  auto e{result()};

  auto fname{node.get_name()};
  res = fname + '(' + e + ')';
}

void NodeVisitorStringifier::visit(const NodeUnary &node) {
  node.get_expr().accept(*this);
  auto e{result()};

  if (node.get_type() != TokenType::MINUS) {
    throw "Unknown unary operation";
  }

  res = "-(" + e + ")";
}

void NodeVisitorStringifier::visit(const NodeValue &node) {
  auto val{node.get_val()};
  if (val == 0.0) {
    res = "0";
  } else if (val.real() == 0.0) {
    res = std::to_string(val.imag()) + 'i';
  } else if (val.imag() == 0.0) {
    res = std::to_string(val.real());
  } else {
    res = std::to_string(val.real()) + '+' + std::to_string(val.imag()) + 'i';
  }
}

void NodeVisitorStringifier::visit(const NodeVariable &node) {
  res = node.get_name();
}
