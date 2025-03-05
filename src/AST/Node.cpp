#include "AST/Node.hpp"

Node::Node(TokenType type) : type{type} {}

TokenType Node::get_type() const { return type; }
