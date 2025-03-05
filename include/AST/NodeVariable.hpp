#pragma once

#include <string>

#include "AST/Node.hpp"

class NodeVariable : public Node {
public:
  NodeVariable(std::string name);

  void accept(NodeVisitor &v) const override;

  const std::string &get_name() const;

#ifndef DIFFERENTIATOR_TEST_PRIVATE
private:
#endif
  std::string name{};
};
