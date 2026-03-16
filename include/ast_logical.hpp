#pragma once

#include "ast_node.hpp"

namespace ast {

// equality

class EqualExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  EqualExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

class NotEqualExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  NotEqualExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

// relational

class LessThanExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  LessThanExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

class GreaterThanExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  GreaterThanExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

class LessEqualExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  LessEqualExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

class GreaterEqualExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  GreaterEqualExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

class LNotExpression : public Node {
 private:
  NodePtr expr_;

 public:
  LNotExpression(NodePtr expr) : expr_(std::move(expr)) {}

  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

// LAnd, LOr
class LAndExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  LAndExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

class LOrExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  LOrExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

}  // namespace ast
