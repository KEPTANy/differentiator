#include "AST/Visitors/NodeVisitorCollector.hpp"

#include <cmath>

#include "AST/Node.hpp"
#include "AST/NodeBinary.hpp"
#include "AST/NodeFunction.hpp"
#include "AST/NodeUnary.hpp"
#include "AST/NodeValue.hpp"
#include "AST/NodeVariable.hpp"

std::complex<double> NodeVisitorCollector::result() const { return res; }

void NodeVisitorCollector::visit(const NodeBinary &node) {
  node.get_left_expr().accept(*this);
  auto l{result()};
  node.get_right_expr().accept(*this);
  auto r{result()};

  auto op{node.get_type()};
  switch (op) {
  case TokenType::PLUS:
    res = l + r;
    break;
  case TokenType::MINUS:
    res = l - r;
    break;
  case TokenType::STAR:
    res = l * r;
    break;
  case TokenType::SLASH:
    res = l / r;
    break;
  case TokenType::POW:
    res = std::pow(l, r);
    break;
  default:
    throw "Unknown binary operation";
  }
}

void NodeVisitorCollector::visit(const NodeFunction &node) {
  node.get_expr().accept(*this);
  auto e{result()};

  auto fname{node.get_name()};
  if (fname == "sin") {
    res = sin(e);
  } else if (fname == "cos") {
    res = cos(e);
  } else if (fname == "exp") {
    res = exp(e);
  } else if (fname == "ln") {
    res = log(e);
  } else {
    throw "Unknown function";
  }
}

void NodeVisitorCollector::visit(const NodeUnary &node) {
  node.get_expr().accept(*this);
  auto e{result()};

  if (node.get_type() != TokenType::MINUS) {
    throw "Unknown unary operation";
  }

  res = -e;
}

void NodeVisitorCollector::visit(const NodeValue &node) {
  res = node.get_val();
}

void NodeVisitorCollector::visit([[maybe_unused]] const NodeVariable &node) {
  throw "Can not collect expression with variables in it";
}
