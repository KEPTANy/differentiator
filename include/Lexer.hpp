#ifndef DIFFERENTIATOR_LEXER_HPP
#define DIFFERENTIATOR_LEXER_HPP

#include <cstddef>
#include <string>

#include "Token.hpp"

class Lexer {
public:
  explicit Lexer(std::string expr);
  Token get_next_token();

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::string source;
  std::size_t pos;

  void skip_whitespace();
};

#endif
