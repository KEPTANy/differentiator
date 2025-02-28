#ifndef DIFFERENTIATOR_TOKEN_HPP
#define DIFFERENTIATOR_TOKEN_HPP

#include <string_view>

enum class TokenType {
  END, // indicates that there are no more tokens

  OPERATOR,  // +, -, *, ^, /, =, (, )
  SYMBOL, // function or variable
  NUMBER, // number literal

  BAD_TOKEN // generated in case of an error
};

struct Token {
  TokenType type;
  std::string_view str;
};

#endif
