#pragma once

#include <string>

enum class TokenType {
  END, // indicates that there are no more tokens

  PLUS,
  MINUS,
  STAR,
  SLASH,
  POW,
  LPAR,
  RPAR,
  EQ,

  SYMBOL, // function or variable
  NUMBER, // number literal

  BAD_TOKEN // generated in case of an error
};

struct Token {
  TokenType type;
  std::string str{};
};
