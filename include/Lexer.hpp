#ifndef DIFFERENTIATOR_LEXER_HPP
#define DIFFERENTIATOR_LEXER_HPP

#include <cstddef>
#include <string>

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

class Lexer {
public:
  explicit Lexer(std::string expr);
  Token get_next_token();

private:
  std::string source;
  std::size_t pos;

  void skip_whitespace();
};

#endif
