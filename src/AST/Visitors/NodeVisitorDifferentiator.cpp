#include "AST/Visitors/NodeVisitorDifferentiator.hpp"

#include <memory>

#include "AST/NodeBinary.hpp"
#include "AST/NodeFunction.hpp"
#include "AST/NodeUnary.hpp"
#include "AST/NodeValue.hpp"
#include "AST/NodeVariable.hpp"
#include "AST/Visitors/NodeVisitorReplicator.hpp"

NodeVisitorDifferentiator::NodeVisitorDifferentiator(std::string wrt)
    : wrt{wrt} {}

std::unique_ptr<Node> NodeVisitorDifferentiator::result() {
  return std::move(res);
}

void NodeVisitorDifferentiator::visit(const NodeBinary &node) {
  node.get_left_expr().accept(*this);
  auto l_diff{result()};

  node.get_right_expr().accept(*this);
  auto r_diff{result()};

  if (node.get_type() == TokenType::PLUS) {
    res = std::make_unique<NodeBinary>(std::move(l_diff), TokenType::PLUS,
                                       std::move(r_diff));
  } else if (node.get_type() == TokenType::MINUS) {
    res = std::make_unique<NodeBinary>(std::move(l_diff), TokenType::MINUS,
                                       std::move(r_diff));
  } else if (node.get_type() == TokenType::STAR) {
    // d(f*g))/dx -> g*df/dx + f*dg/dx
    NodeVisitorReplicator vis{};
    node.get_left_expr().accept(vis);
    auto l{vis.result()};

    node.get_right_expr().accept(vis);
    auto r{vis.result()};

    auto full_l = std::make_unique<NodeBinary>(std::move(r), TokenType::STAR,
                                               std::move(l_diff));
    auto full_r = std::make_unique<NodeBinary>(std::move(l), TokenType::STAR,
                                               std::move(r_diff));

    res = std::make_unique<NodeBinary>(std::move(full_l), TokenType::PLUS,
                                       std::move(full_r));
  } else if (node.get_type() == TokenType::SLASH) {
    // d(f/g) -> (g*(df)/dx - f*(dg)/dx)/(g^2)
    NodeVisitorReplicator vis{};
    node.get_left_expr().accept(vis);
    auto l{vis.result()};

    node.get_right_expr().accept(vis);
    auto r1{vis.result()};

    node.get_right_expr().accept(vis);
    auto r2{vis.result()};

    r2 = std::make_unique<NodeBinary>(std::move(r2), TokenType::POW,
                                      std::make_unique<NodeValue>(2));

    auto full_l = std::make_unique<NodeBinary>(std::move(r1), TokenType::STAR,
                                               std::move(l_diff));
    auto full_r = std::make_unique<NodeBinary>(std::move(l), TokenType::STAR,
                                               std::move(r_diff));

    res = std::make_unique<NodeBinary>(
        std::make_unique<NodeBinary>(std::move(full_l), TokenType::MINUS,
                                     std::move(full_r)),
        TokenType::SLASH, std::move(r2));
  } else if (node.get_type() == TokenType::POW) {
    // d(f^g)/dx -> d(e^(g*ln(f)))/dx -> f^g*((df/dx)*g/f + (dg/dx)*ln(f))
    NodeVisitorReplicator vis{};
    node.get_left_expr().accept(vis);
    auto l{vis.result()};
    node.get_right_expr().accept(vis);
    auto r{vis.result()};

    auto l_pow_r{std::make_unique<NodeBinary>(std::move(l), TokenType::POW,
                                              std::move(r))};

    node.get_left_expr().accept(vis);
    l = vis.result();
    node.get_right_expr().accept(vis);
    r = vis.result();

    auto left_term{std::make_unique<NodeBinary>(
        std::make_unique<NodeBinary>(std::move(l_diff), TokenType::STAR,
                                     std::move(r)),
        TokenType::SLASH, std::move(l))};

    node.get_left_expr().accept(vis);
    l = vis.result();

    auto right_term{std::make_unique<NodeBinary>(
        std::move(r_diff), TokenType::STAR,
        std::make_unique<NodeFunction>("ln", std::move(l)))};

    auto group{std::make_unique<NodeBinary>(
        std::move(left_term), TokenType::PLUS, std::move(right_term))};

    res = std::make_unique<NodeBinary>(std::move(l_pow_r), TokenType::STAR,
                                       std::move(group));
  } else {
    throw std::invalid_argument("Unknown binary operation");
  }
}

void NodeVisitorDifferentiator::visit(const NodeFunction &node) {
  NodeVisitorReplicator vis{};
  node.get_expr().accept(vis);
  auto arg{vis.result()};

  node.get_expr().accept(*this);
  auto arg_diff{result()};

  std::unique_ptr<Node> func_diff{nullptr};
  if (node.get_name() == "sin") {
    // d(sin(f))/dx -> cos(f) * df/dx
    func_diff = std::make_unique<NodeFunction>("cos", std::move(arg));
  } else if (node.get_name() == "cos") {
    // d(cos(f))/dx -> -sin(f) * df/dx
    auto t{std::make_unique<NodeFunction>("sin", std::move(arg))};
    func_diff = std::make_unique<NodeUnary>(TokenType::MINUS, std::move(t));
  } else if (node.get_name() == "exp") {
    // d(exp(f))/dx -> exp(f) * df/dx
    func_diff = std::make_unique<NodeFunction>("exp", std::move(arg));
  } else if (node.get_name() == "ln") {
    // d(ln(f))/dx -> f^(-1) * df/dx
    auto t{std::make_unique<NodeValue>(-1)};
    func_diff = std::make_unique<NodeBinary>(std::move(arg), TokenType::POW,
                                             std::move(t));
  } else {
    throw std::invalid_argument("Unknown function");
  }
  res = std::make_unique<NodeBinary>(std::move(func_diff), TokenType::STAR,
                                     std::move(arg_diff));
}

void NodeVisitorDifferentiator::visit(const NodeUnary &node) {
  if (node.get_type() != TokenType::MINUS) {
    throw std::invalid_argument("Unknown unary operation");
  }

  node.get_expr().accept(*this);
  auto expr_diff{result()};

  res = std::make_unique<NodeUnary>(TokenType::MINUS, std::move(expr_diff));
}

void NodeVisitorDifferentiator::visit([[maybe_unused]] const NodeValue &node) {
  res = std::make_unique<NodeValue>(0);
}

void NodeVisitorDifferentiator::visit(const NodeVariable &node) {
  if (node.get_name() == wrt) {
    res = std::make_unique<NodeValue>(1);
  } else {
    res = std::make_unique<NodeValue>(0);
  }
}
