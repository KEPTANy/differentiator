#include "AST/NodeBinary.hpp"

NodeBinary::NodeBinary(std::unique_ptr<Node> l, TokenType type,
                       std::unique_ptr<Node> r)
    : Node{type}, left{std::move(l)}, right{std::move(r)} {}

void NodeBinary::accept(NodeVisitor &v) const { v.visit(*this); }

const Node &NodeBinary::get_left_expr() const { return *left; }

const Node &NodeBinary::get_right_expr() const { return *right; }
