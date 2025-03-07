#include "AST/Visitors/NodeVisitorStringifier.hpp"

#include <format>

#include "AST/Node.hpp"
#include "AST/NodeBinary.hpp"
#include "AST/NodeFunction.hpp"
#include "AST/NodeUnary.hpp"
#include "AST/NodeValue.hpp"
#include "AST/NodeVariable.hpp"

std::string NodeVisitorStringifier::result() const { return res; }

static int prec(TokenType t) {
  switch (t) {
  case TokenType::PLUS:
  case TokenType::MINUS:
    return 1;
  case TokenType::STAR:
  case TokenType::SLASH:
    return 2;
  case TokenType::POW:
    return 3;
  default:
    return 4;
  }
}

void NodeVisitorStringifier::visit(const NodeBinary &node) {
  node.get_left_expr().accept(*this);
  auto l{result()};
  auto l_prec{precedence};

  node.get_right_expr().accept(*this);
  auto r{result()};
  auto r_prec{precedence};

  auto op{node.get_type()};
  precedence = prec(op);

  if (l_prec < precedence) {
    l = '(' + l + ')';
  }

  if (r[0] == '-' || r_prec < precedence) {
    r = '(' + r + ')';
  }

  switch (op) {
  case TokenType::PLUS:
    res = l + '+' + r;
    break;
  case TokenType::MINUS:
    res = l + '-' + r;
    break;
  case TokenType::STAR:
    res = l + '*' + r;
    break;
  case TokenType::SLASH:
    res = l + '/' + r;
    break;
  case TokenType::POW:
    res = l + '^' + r;
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
  precedence = prec(TokenType::SYMBOL);
}

void NodeVisitorStringifier::visit(const NodeUnary &node) {
  node.get_expr().accept(*this);
  auto e{result()};

  if (node.get_type() != TokenType::MINUS) {
    throw "Unknown unary operation";
  }

  res = '-' + ((precedence <= prec(TokenType::STAR)) ? '(' + res + ')' : res);
  precedence = prec(TokenType::STAR); // treat as multiplication by -1
}

void NodeVisitorStringifier::visit(const NodeValue &node) {
  auto val{node.get_val()};

  auto real{val.real()};
  auto imag{val.imag()};
  if (real != 0 && imag != 0) {
    precedence = prec(TokenType::PLUS); // string has an addition
    res = std::format("{}+{}*i", real, imag);
    return;
  }

  if (imag != 0) {
    precedence = prec(TokenType::STAR); // string has a multiplication
    res = std::format("{}*i", imag);
    return;
  }

  precedence = prec(TokenType::NUMBER); // only a number
  res = std::format("{}", real);
}

void NodeVisitorStringifier::visit(const NodeVariable &node) {
  res = node.get_name();
  precedence = prec(TokenType::SYMBOL);
}
