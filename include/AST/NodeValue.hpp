#pragma once

#include <complex>

#include "AST/Node.hpp"

class NodeValue : public Node {
public:
  NodeValue(std::complex<double> val);

  void accept(NodeVisitor &v) const override;

  std::complex<double> get_val() const;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::complex<double> val{};
};
