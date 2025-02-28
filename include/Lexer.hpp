#ifndef DIFFERENTIATOR_LEXER_HPP
#define DIFFERENTIATOR_LEXER_HPP

#include <cstddef>
#include <string_view>

#include "Token.hpp"

class Lexer {
public:
  explicit Lexer(std::string_view expr);
  Token get_next_token();

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::string_view source;
  std::size_t pos;

  Token get_symbol_token();
  Token get_number_token();

  void skip_whitespace();
};

#endif
