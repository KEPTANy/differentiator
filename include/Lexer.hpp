#pragma once

#include <cstddef>
#include <string>

#include "Token.hpp"

class Lexer {
public:
  Lexer(std::string expr);

  Token get_next_token();

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::size_t pos;
  std::string source{};

  Token get_symbol_token();
  Token get_number_token();

  void skip_whitespace();
};
