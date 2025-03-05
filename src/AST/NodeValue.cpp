#include "AST/NodeValue.hpp"

NodeValue::NodeValue(std::complex<double> val) : Node(TokenType::NUMBER), val{val} {}

void NodeValue::accept(NodeVisitor &v) const { v.visit(*this); }

std::complex<double> NodeValue::get_val() const { return val; }
