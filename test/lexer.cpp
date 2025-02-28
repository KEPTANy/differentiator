#include "doctest.h"

#include "Lexer.hpp"
#include <vector>

TEST_CASE("Lexer") {
  using enum TokenType;

  Lexer lexer("");

  auto tokens = [&lexer](std::string_view expr) -> std::vector<Token> {
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

  auto views = [](std::vector<Token> toks) -> std::vector<std::string_view> {
    std::vector<std::string_view> res;
    for (auto t : toks) {
      res.emplace_back(t.str);
    }
    return res;
  };

  auto a = tokens("123141x*ln(10)^x^x");
  CHECK(types(a) == std::vector<TokenType>{
    NUMBER, SYMBOL, OPERATOR, SYMBOL, OPERATOR, NUMBER,
    OPERATOR, OPERATOR, SYMBOL, OPERATOR, SYMBOL, END
  });

  CHECK(views(a) == std::vector<std::string_view>{
    "123141", "x", "*", "ln", "(", "10", ")", "^", "x", "^", "x", {}
  });

  a = tokens("sqrt(x*y^z)/1442.03");
  CHECK(types(a) == std::vector<TokenType>{
    SYMBOL, OPERATOR, SYMBOL, OPERATOR, SYMBOL, OPERATOR,
    SYMBOL, OPERATOR, OPERATOR, NUMBER, END
  });

  CHECK(views(a) == std::vector<std::string_view>{
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
