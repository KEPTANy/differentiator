#pragma once

#include <memory>

#include "AST/Node.hpp"
#include "Lexer.hpp"

class Parser : private Lexer {
public:
  Parser(std::string expr);

  std::unique_ptr<Node> parse_expression();
  std::pair<std::string, std::unique_ptr<Node>> parse_assignment();

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::unique_ptr<Node> expr();
  std::unique_ptr<Node> sum();
  std::unique_ptr<Node> unary();
  std::unique_ptr<Node> mul();
  std::unique_ptr<Node> pow();
  std::unique_ptr<Node> primary();
  std::unique_ptr<Node> func_call();
  std::unique_ptr<Node> group();
};
