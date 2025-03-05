#include "AST/NodeFunction.hpp"

NodeFunction::NodeFunction(std::string name, std::unique_ptr<Node> expr)
    : Node(TokenType::SYMBOL), name{name}, expr{std::move(expr)} {}

void NodeFunction::accept(NodeVisitor &v) const { v.visit(*this); }

const Node &NodeFunction::get_expr() const { return *expr; }
