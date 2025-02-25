#include "Lexer.hpp"

#include <cctype>
#include <cstring>

Lexer::Lexer(std::string expr) : source{expr}, pos{0} {}

Token Lexer::get_next_token() {
  skip_whitespace();

  if (pos >= source.size()) {
    return Token(TokenType::END, {});
  }

  if (std::strchr("+-*/=()", source[pos]) != nullptr) {
    return {TokenType::OPERATOR, {source.data() + pos++, 1}};
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

    return {TokenType::NUMBER, {source.data() + start, pos - start}};
  }
  
  return {TokenType::BAD_TOKEN, {source.data() + pos++, 1}};
}

void Lexer::skip_whitespace() {
  while (pos < source.size() && std::isspace(source[pos])) {
    pos++;
  }
}
