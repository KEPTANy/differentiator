#include "Parser.hpp"

#include <stdexcept>
#include <string>

#include "AST/Node.hpp"
#include "AST/NodeBinary.hpp"
#include "AST/NodeFunction.hpp"
#include "AST/NodeUnary.hpp"
#include "AST/NodeValue.hpp"
#include "AST/NodeVariable.hpp"

Parser::Parser(std::string expr) : Lexer{expr} {}

std::unique_ptr<Node> Parser::parse_expression() {
  auto res{expr()};
  if (get_next_token().type != TokenType::END) {
    throw std::invalid_argument("Can not parse expression");
  }
  return res;
}

std::pair<std::string, std::unique_ptr<Node>> Parser::parse_assignment() {
  auto v{get_next_token()};

  auto eq{get_next_token()};
  if (eq.type != TokenType::EQ || v.type != TokenType::SYMBOL || v.str == "i" ||
      v.str == "sin" || v.str == "cos" || v.str == "exp" || v.str == "ln") {
    throw std::invalid_argument("Can not parse assignment");
  }

  auto e{parse_expression()};
  return {v.str, std::move(e)};
}

std::unique_ptr<Node> Parser::expr() { return sum(); }

std::unique_ptr<Node> Parser::sum() {
  auto left{unary()};

  auto tok{get_next_token()};
  while (tok.type == TokenType::PLUS || tok.type == TokenType::MINUS) {
    left = std::make_unique<NodeBinary>(std::move(left), tok.type, mul());

    tok = get_next_token();
  }

  put_back(tok);

  return left;
}

std::unique_ptr<Node> Parser::unary() {
  auto tok{get_next_token()};
  if (tok.type == TokenType::MINUS) {
    return std::make_unique<NodeUnary>(tok.type, mul());
  }

  put_back(tok);

  return mul();
}

std::unique_ptr<Node> Parser::mul() {
  auto left{pow()};

  while (true) {
    auto tok{get_next_token()};
    if (tok.type == TokenType::STAR || tok.type == TokenType::SLASH) {
      left = std::make_unique<NodeBinary>(std::move(left), tok.type, pow());
    } else if (tok.type == TokenType::SYMBOL || tok.type == TokenType::LPAR) {
      // if there's no tokens inbetween two pow expressions, TokenType::STAR is
      // implied. right side is never a number.
      put_back(tok);
      left =
          std::make_unique<NodeBinary>(std::move(left), TokenType::STAR, pow());
    } else {
      put_back(tok);
      break;
    }
  }

  return left;
}

std::unique_ptr<Node> Parser::pow() {
  auto left{primary()};

  auto tok{get_next_token()};
  if (tok.type == TokenType::POW) {
    return std::make_unique<NodeBinary>(std::move(left), tok.type, pow());
  }

  put_back(tok);

  return left;
}

std::unique_ptr<Node> Parser::primary() {
  auto tok{get_next_token()};
  if (tok.type == TokenType::LPAR) {
    put_back(tok);
    return group();
  } else if (tok.type == TokenType::NUMBER) {
    return std::make_unique<NodeValue>(std::stod(tok.str));
  } else if (tok.type == TokenType::SYMBOL) {
    if (tok.str == "sin" || tok.str == "cos" || tok.str == "exp" ||
        tok.str == "ln") {
      put_back(tok);
      return func_call();
    }
    if (tok.str == "i") {
      return std::make_unique<NodeValue>(std::complex<double>(0.0, 1.0));
    }
    return std::make_unique<NodeVariable>(tok.str);
  }
  throw std::invalid_argument("Can not parse primary expression");
}

std::unique_ptr<Node> Parser::func_call() {
  auto name{get_next_token()};
  if (name.type != TokenType::SYMBOL ||
      (name.str != "sin" && name.str != "cos" && name.str != "exp" &&
       name.str != "ln")) {
    throw std::invalid_argument("Can not parse function call");
  }

  return std::make_unique<NodeFunction>(name.str, group());
}

std::unique_ptr<Node> Parser::group() {
  if (get_next_token().type != TokenType::LPAR) {
    throw std::invalid_argument("Can not parse group expression");
  }

  auto res{expr()};

  if (get_next_token().type != TokenType::RPAR) {
    throw std::invalid_argument("Can not parse group expression");
  }

  return res;
}
