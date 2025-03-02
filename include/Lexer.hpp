#pragma once

#include <cstddef>
#include <stack>
#include <string>

#include "Token.hpp"

class Lexer {
public:
  Lexer(std::string expr);

  Token get_next_token();

  void put_back(Token token);

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::string source{};
  std::size_t pos{};
  std::stack<Token> reserved{};

  Token get_symbol_token();
  Token get_number_token();

  void skip_whitespace();
};
