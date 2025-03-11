#include "Expression.hpp"

#include "AST/NodeBinary.hpp"
#include "AST/NodeFunction.hpp"
#include "AST/NodeUnary.hpp"
#include "AST/Visitors/NodeVisitorCollector.hpp"
#include "AST/Visitors/NodeVisitorReplicator.hpp"
#include "AST/Visitors/NodeVisitorSimplifier.hpp"
#include "AST/Visitors/NodeVisitorStringifier.hpp"
#include "AST/Visitors/NodeVisitorSubstitutor.hpp"
#include "AST/Visitors/NodeVisitorDifferentiator.hpp"
#include "Parser.hpp"
#include "Token.hpp"

Expression::Expression(const Expression &other) { *this = other; }

Expression &Expression::operator=(const Expression &other) {
  return *this = other.copy();
}

Expression::Expression() : expr{nullptr} {}

Expression::Expression(std::unique_ptr<Node> e) : expr{std::move(e)} {}

Expression::Expression(std::string str)
    : Expression{Parser(str).parse_expression()} {}

const Node &Expression::get_expr() const { return *expr; }

Expression Expression::copy() const {
  NodeVisitorReplicator vis{};
  get_expr().accept(vis);
  return vis.result();
}

std::string Expression::to_string() const {
  NodeVisitorStringifier vis{};
  get_expr().accept(vis);
  return vis.result();
}

std::complex<double> Expression::collect() const {
  NodeVisitorCollector vis{};
  get_expr().accept(vis);
  return vis.result();
}

Expression Expression::substitute(std::string var_name,
                                  const Expression &expr) const {
  NodeVisitorSubstitutor vis{var_name, *expr.expr};
  get_expr().accept(vis);
  return vis.result();
}

Expression Expression::simplify() const {
  NodeVisitorSimplifier vis{};
  get_expr().accept(vis);
  return vis.result();
}

Expression Expression::diff(std::string wrt) const {
  NodeVisitorDifferentiator vis{wrt};
  get_expr().accept(vis);
  return vis.result();
}

Expression Expression::operator+(const Expression &rhs) const {
  return Expression(*this) += rhs;
}

Expression &Expression::operator+=(const Expression &rhs) {
  expr = std::make_unique<NodeBinary>(std::move(expr), TokenType::PLUS,
                                      Expression(rhs).expr);
  return *this;
}

Expression Expression::operator-() const {
  return Expression(std::make_unique<NodeUnary>(TokenType::MINUS, copy().expr));
}

Expression Expression::operator-(const Expression &rhs) const {
  return Expression(*this) -= rhs;
}

Expression &Expression::operator-=(const Expression &rhs) {
  expr = std::make_unique<NodeBinary>(std::move(expr), TokenType::MINUS,
                                      Expression(rhs).expr);
  return *this;
}

Expression Expression::operator*(const Expression &rhs) const {
  return Expression(*this) *= rhs;
}

Expression &Expression::operator*=(const Expression &rhs) {
  expr = std::make_unique<NodeBinary>(std::move(expr), TokenType::STAR,
                                      Expression(rhs).expr);
  return *this;
}

Expression Expression::operator/(const Expression &rhs) const {
  return Expression(*this) /= rhs;
}

Expression &Expression::operator/=(const Expression &rhs) {
  expr = std::make_unique<NodeBinary>(std::move(expr), TokenType::SLASH,
                                      Expression(rhs).expr);
  return *this;
}

Expression Expression::operator^(const Expression &rhs) const {
  return Expression(*this) ^= rhs;
}

Expression &Expression::operator^=(const Expression &rhs) {
  expr = std::make_unique<NodeBinary>(std::move(expr), TokenType::POW,
                                      Expression(rhs).expr);
  return *this;
}

Expression sin(const Expression &x) {
  return Expression(std::make_unique<NodeFunction>("sin", Expression(x).expr));
}

Expression cos(const Expression &x) {
  return Expression(std::make_unique<NodeFunction>("cos", Expression(x).expr));
}

Expression exp(const Expression &x) {
  return Expression(std::make_unique<NodeFunction>("exp", Expression(x).expr));
}

Expression ln(const Expression &x) {
  return Expression(std::make_unique<NodeFunction>("ln", Expression(x).expr));
}
