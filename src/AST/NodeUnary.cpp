#include "AST/NodeUnary.hpp"

NodeUnary::NodeUnary(TokenType type, std::unique_ptr<Node> expr)
    : Node(type), expr{std::move(expr)} {}

void NodeUnary::accept(NodeVisitor &v) const { v.visit(*this); }

const Node &NodeUnary::get_expr() const { return *expr; }
