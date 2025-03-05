#pragma once

#include <complex>

#include "AST/Visitors/NodeVisitor.hpp"

class NodeVisitorCollector : public NodeVisitor {
public:
  ~NodeVisitorCollector() override = default;

  std::complex<double> result() const;

  void visit(const NodeBinary &node) override;
  void visit(const NodeFunction &node) override;
  void visit(const NodeUnary &node) override;
  void visit(const NodeValue &node) override;
  void visit(const NodeVariable &node) override;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::complex<double> res{};
};
