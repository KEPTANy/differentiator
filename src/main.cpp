#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "AST/NodeVariable.hpp"
#include "Expression.hpp"
#include "Parser.hpp"

[[noreturn]] void help() {
  std::cout << "Usage:\n"
               "./differentiator --eval <expression> [<substitutions>]\n"
               "./differentiator --diff <expression> --by <variable>\n"
               "\n"
               "Examples:\n"
               "./differentiator --eval \"x * y\" x=10 y=12\n"
               "./differentiator --diff \"x * sin(x)\" --by x\n";
  exit(EXIT_FAILURE);
}

enum class TargetType { NOT_SPECIFIED = 0, DIFF, EVAL };

struct Target {
  TargetType type{TargetType::NOT_SPECIFIED};
  std::string expr{};
  std::vector<std::string> subs{};
  std::string wrt{};
};

void diff(Target &t) {
  Expression expr;
  try {
    expr = t.expr;
  } catch (...) {
    std::cout << "Failed to parse \"" << t.expr << "\"\n";
    exit(EXIT_FAILURE);
  }

  std::string wrt;
  try {
    Expression temp(t.wrt);
    wrt = dynamic_cast<const NodeVariable &>(temp.get_expr()).get_name();
  } catch (...) {
    std::cout << "Failed to parse \"" << t.wrt << "\"\n";
    exit(EXIT_FAILURE);
  }

  // clang-format off
  std::cout << "Operation: diff\n"
               "With respect to:\n"
               "\t" << wrt << "\n"
               "Expression:\n"
               "\t" << expr.to_string() << "\n";

  expr = expr.simplify();

  std::cout << "Simplified:\n"
               "\t" << expr.to_string() << "\n";

  std::cout << "########################\n";
  // clang-format on

  expr = expr.diff(wrt);
  std::cout << "Differentiated: \n"
               "\t" << expr.to_string() << "\n";

  expr = expr.simplify();
  std::cout << "After simplifications:\n"
               "\t" << expr.to_string() << "\n"
               "------------------------\n";

  std::cout << "Final result: " << expr.to_string() << "\n";
}

void eval(Target &t) {
  Expression expr;
  try {
    expr = t.expr;
  } catch (...) {
    std::cout << "Failed to parse \"" << t.expr << "\"\n";
    exit(EXIT_FAILURE);
  }

  std::vector<std::pair<std::string, Expression>> subs;
  for (auto &s : t.subs) {
    try {
      subs.emplace_back(Parser(s).parse_assignment());
    } catch (...) {
      std::cout << "Failed to parse \"" << s << "\"\n";
      exit(EXIT_FAILURE);
    }
  }

  // clang-format off
  std::cout << "Operation: eval\n"
               "Expression:\n"
               "\t" << expr.to_string() << "\n";

  expr = expr.simplify();

  std::cout << "Simplified:\n"
               "\t" << expr.to_string() << "\n"
               "Substitutions:\n";

  for (auto [s, e] : subs) {
    std::cout << "\t" << s << " = " << e.to_string() << "\n";
  }

  std::cout << "########################\n";
  // clang-format on

  for (std::size_t i{0}; i < subs.size(); i++) {
    auto [s, e] = subs[i];

    std::cout << "Substitution: \n"
                 "\t" << s << " = " << e.to_string() << "\n";

    expr = expr.substitute(s, e);
    std::cout << "Expression after substitution:\n"
                 "\t" << expr.to_string() << "\n";

    expr = expr.simplify();
    std::cout << "After simplifications:\n"
                 "\t" << expr.to_string() << "\n"
                 "------------------------\n";
  }

  std::cout << "Final result: " << expr.to_string() << "\n";
}

int main(int argc, const char *argv[]) {
  if (argc < 3) {
    help();
  }

  Target t;
  t.expr = argv[2];
  if (argv[1] == std::string("--diff")) {
    if (argc != 5 || argv[3] != std::string("--by")) {
      help();
    }
    t.type = TargetType::DIFF;
    t.wrt = argv[4];
  } else if (argv[1] == std::string("--eval")) {
    t.type = TargetType::EVAL;
    for (int i{3}; i < argc; i++) {
      t.subs.emplace_back(argv[i]);
    }
  } else {
    help();
  }

  if (t.type == TargetType::DIFF) {
    diff(t);
  } else if (t.type == TargetType::EVAL) {
    eval(t);
  }

  return EXIT_SUCCESS;
}
