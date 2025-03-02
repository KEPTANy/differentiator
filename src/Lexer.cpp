#include "Lexer.hpp"

#include <cctype>

Lexer::Lexer(std::string expr) : source{expr}, pos{0} {}

Token Lexer::get_next_token() {
  if (!reserved.empty()) {
    Token res{reserved.top()};
    reserved.pop();
    return res;
  }

  skip_whitespace();

  if (pos >= source.size()) {
    return Token{TokenType::END, ""};
  }

  if (std::isalpha(source[pos])) {
    return get_symbol_token();
  }

  if (std::isdigit(source[pos])) {
    return get_number_token();
  }

  Token tok{TokenType::BAD_TOKEN, source.substr(pos++, 1)};
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

void Lexer::put_back(Token token) { reserved.push(token); }

Token Lexer::get_symbol_token() {
  std::size_t start{pos};
  while (pos < source.size() && std::isalpha(source[pos])) {
    pos++;
  }

  return Token{TokenType::SYMBOL, source.substr(start, pos - start)};
}

Token Lexer::get_number_token() {
  std::size_t start{pos};
  while (pos < source.size() && std::isdigit(source[pos])) {
    pos++;
  }

  if (pos < source.size() && source[pos] == '.') {
    pos++;

    while (pos < source.size() && std::isdigit(source[pos])) {
      pos++;
    }
  }

  return Token{source[pos - 1] != '.' ? TokenType::NUMBER
                                      : TokenType::BAD_TOKEN,
               source.substr(start, pos - start)};
}

void Lexer::skip_whitespace() {
  while (pos < source.size() && std::isspace(source[pos])) {
    pos++;
  }
}
