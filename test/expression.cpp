#include "doctest.h"

#include "Expression.hpp"

TEST_CASE("Expression") {
  SUBCASE("Func values") {
    CHECK(sin(Expression("0")).simplify().to_string() == "0.000");
    CHECK(sin(Expression("3.1415/2")).simplify().to_string() == "1.000");
    CHECK(sin(Expression("-3.1415/2")).simplify().to_string() == "-1.000");
    CHECK(sin(Expression("3.1415")).simplify().to_string() == "0.000");

    CHECK(cos(Expression("0")).simplify().to_string() == "1.000");
    CHECK(cos(Expression("3.1415/2")).simplify().to_string() == "0.000");
    CHECK(cos(Expression("-3.1415/2")).simplify().to_string() == "0.000");
    CHECK(cos(Expression("3.1415")).simplify().to_string() == "-1.000");

    CHECK(ln(Expression("1")).simplify().to_string() == "0.000");
    CHECK(ln(Expression("2.7182")).simplify().to_string() == "1.000");
    CHECK(ln(Expression("1/2.7182")).simplify().to_string() == "-1.000");

    CHECK(exp(Expression("1")).simplify().to_string() == "2.718");
    CHECK(exp(Expression("2")).simplify().to_string() == "7.389");
    CHECK(exp(Expression("-1")).simplify().to_string() == "0.368");
  }

  SUBCASE("Diff") {
    SUBCASE("x*sin(x)") {
      Expression expr("x sin(x)");
      expr = expr.diff("x").simplify();
      CHECK(expr.substitute("x", 0).simplify().to_string() == "0.000");
      CHECK(expr.substitute("x", 8).simplify().to_string() == "-0.175");
      CHECK(expr.substitute("x", -8).simplify().to_string() == "0.175");
    }

    SUBCASE("cos(x^(-0.5))") {
      Expression expr("cos(x^(-0.5))");
      expr = expr.diff("x").simplify();
      CHECK(expr.substitute("x", 3).simplify().to_string() == "0.053");
      CHECK(expr.substitute("x", 4).simplify().to_string() == "0.030");
    }

    SUBCASE("x^x") {
      Expression expr("x^x");
      expr = expr.diff("x").simplify();
      CHECK(expr.substitute("x", Expression("1/2.718")).simplify().to_string() == "0.000");
      CHECK(expr.substitute("x", 1).simplify().to_string() == "1.000");
    }

    SUBCASE("(x^2+3)/(1-x^3)^(1/3)") {
      Expression expr("(x^2+3)/(1-x^3)^(1/3)");
      expr = expr.diff("x").simplify();
    }
  }
}
