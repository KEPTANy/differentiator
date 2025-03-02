#include "doctest.h"

#include "Lexer.hpp"

#include <vector>

TEST_CASE("Lexer") {
  using enum TokenType;

  Lexer lexer("");

  auto tokens = [&lexer](std::string expr) -> std::vector<Token> {
    std::vector<Token> res;
    lexer = Lexer(expr);
    do {
      res.emplace_back(lexer.get_next_token());
    } while (res.back().type != END);
    return res;
  };

  auto types = [](std::vector<Token> toks) -> std::vector<TokenType> {
    std::vector<TokenType> res;
    for (auto t : toks) {
      res.emplace_back(t.type);
    }
    return res;
  };

  auto strings = [](std::vector<Token> toks) -> std::vector<std::string> {
    std::vector<std::string> res;
    for (auto t : toks) {
      res.emplace_back(t.str);
    }
    return res;
  };

  auto a = tokens("123141x*ln(10)^x^x");
  CHECK(types(a) == std::vector<TokenType>{
    NUMBER, SYMBOL, STAR, SYMBOL, LPAR, NUMBER,
    RPAR, POW, SYMBOL, POW, SYMBOL, END
  });

  CHECK(strings(a) == std::vector<std::string>{
    "123141", "x", "*", "ln", "(", "10", ")", "^", "x", "^", "x", {}
  });

  a = tokens("sqrt(x*y^z)/1442.03");
  CHECK(types(a) == std::vector<TokenType>{
    SYMBOL, LPAR, SYMBOL, STAR, SYMBOL, POW,
    SYMBOL, RPAR, SLASH, NUMBER, END
  });

  CHECK(strings(a) == std::vector<std::string>{
    "sqrt", "(", "x", "*", "y", "^", "z", ")", "/", "1442.03", {}
  });

  lexer = Lexer(".012");
  CHECK(lexer.get_next_token().type == BAD_TOKEN);

  lexer = Lexer("12.55.012");
  CHECK(lexer.get_next_token().str == "12.55");
  CHECK(lexer.get_next_token().type == BAD_TOKEN);

  lexer = Lexer("12.");
  CHECK(lexer.get_next_token().type == BAD_TOKEN);
}
