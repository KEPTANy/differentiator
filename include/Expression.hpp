#pragma once

#include <complex>
#include <concepts>
#include <memory>

#include "AST/Node.hpp"
#include "AST/NodeValue.hpp"

class Expression {
public:
  ~Expression() = default;
  Expression(const Expression &other);
  Expression &operator=(const Expression &other);
  Expression(Expression &&other) = default;
  Expression &operator=(Expression &&other) = default;

  template <typename T>
    requires std::convertible_to<T, std::complex<double>>
  Expression(T val) : expr{std::make_unique<NodeValue>(val)} {}

  Expression();
  Expression(std::unique_ptr<Node> e);
  Expression(std::string str);

  const Node &get_expr() const;

  Expression copy() const;
  std::string to_string() const;
  std::complex<double> collect() const;
  Expression substitute(std::string var_name, const Expression &expr) const;
  Expression simplify() const;
  Expression diff(std::string wrt) const;

  Expression operator+(const Expression &rhs) const;
  Expression &operator+=(const Expression &rhs);

  Expression operator-() const;
  Expression operator-(const Expression &rhs) const;
  Expression &operator-=(const Expression &rhs);

  Expression operator*(const Expression &rhs) const;
  Expression &operator*=(const Expression &rhs);

  Expression operator/(const Expression &rhs) const;
  Expression &operator/=(const Expression &rhs);

  Expression operator^(const Expression &rhs) const;
  Expression &operator^=(const Expression &rhs);

  friend Expression sin(const Expression &x);
  friend Expression cos(const Expression &x);
  friend Expression exp(const Expression &x);
  friend Expression ln(const Expression &x);

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::unique_ptr<Node> expr{};
};

Expression sin(const Expression &x);
Expression cos(const Expression &x);
Expression exp(const Expression &x);
Expression ln(const Expression &x);
