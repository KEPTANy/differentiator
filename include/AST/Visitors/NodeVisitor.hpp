#pragma once

class Node;
class NodeBinary;
class NodeFunction;
class NodeUnary;
class NodeValue;
class NodeVariable;

class NodeVisitor {
public:
  virtual ~NodeVisitor() = default;

  virtual void visit(const NodeBinary &node) = 0;
  virtual void visit(const NodeFunction &node) = 0;
  virtual void visit(const NodeUnary &node) = 0;
  virtual void visit(const NodeValue &node) = 0;
  virtual void visit(const NodeVariable &node) = 0;
};
