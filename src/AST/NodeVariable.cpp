#include "AST/NodeVariable.hpp"

NodeVariable::NodeVariable(std::string name)
    : Node(TokenType::SYMBOL), name{name} {}

void NodeVariable::accept(NodeVisitor &v) const { v.visit(*this); }

const std::string &NodeVariable::get_name() const { return name; }
