#include "Lexer.hpp"

#include <cctype>

Lexer::Lexer(std::string_view expr) : source{expr}, pos{0} {}

Token Lexer::get_next_token() {
  skip_whitespace();

  if (pos >= source.size()) {
    return Token(TokenType::END, {});
  }

  if (std::isalpha(source[pos])) {
    std::size_t start = pos;
    while (pos < source.size() && std::isalpha(source[pos])) {
      pos++;
    }

    return {TokenType::SYMBOL, {source.data() + start, pos - start}};
  }

  if (std::isdigit(source[pos])) {
    std::size_t start = pos;
    bool has_dot = false;
    while (pos < source.size() &&
           (std::isdigit(source[pos]) || (!has_dot && source[pos] == '.'))) {
      if (source[pos] == '.') {
        has_dot = true;
      }
      pos++;
    }

    if (source[pos - 1] != '.') {
      return {TokenType::NUMBER, {source.data() + start, pos - start}};
    }
  }

  Token tok{TokenType::BAD_TOKEN, {source.data() + pos++, 1}};
  switch (source[pos - 1]) {
  case '+':
    tok.type = TokenType::PLUS;
    break;
  case '-':
    tok.type = TokenType::MINUS;
    break;
  case '*':
    tok.type = TokenType::STAR;
    break;
  case '/':
    tok.type = TokenType::SLASH;
    break;
  case '^':
    tok.type = TokenType::POW;
    break;
  case '(':
    tok.type = TokenType::LPAR;
    break;
  case ')':
    tok.type = TokenType::RPAR;
    break;
  case '=':
    tok.type = TokenType::EQ;
    break;
  }

  return tok;
}

void Lexer::skip_whitespace() {
  while (pos < source.size() && std::isspace(source[pos])) {
    pos++;
  }
}
