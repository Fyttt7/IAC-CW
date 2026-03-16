#pragma once

#include "ast_node.hpp"

namespace ast {

class AddExpression : public Node {
 private:
  NodePtr left_;
  NodePtr right_;

 public:
  AddExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}

  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
  std::string GetBaseName() const override { return left_->GetBaseName(); }
};

class SubExpression : public Node {
 private:
  NodePtr left_;
  NodePtr right_;

 public:
  SubExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

class MulExpression : public Node {
 private:
  NodePtr left_;
  NodePtr right_;

 public:
  MulExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

class DivExpression : public Node {
 private:
  NodePtr left_;
  NodePtr right_;

 public:
  DivExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

class ModExpression : public Node {
 private:
  NodePtr left_;
  NodePtr right_;

 public:
  ModExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

// bitwise

class ShiftLeftExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  ShiftLeftExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  void EmitRISC(std::ostream& s, Context& c) const override;
  void Print(std::ostream& s) const override;
};

class ShiftRightExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  ShiftRightExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  void EmitRISC(std::ostream& s, Context& c) const override;
  void Print(std::ostream& s) const override;
};

class BitAndExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  BitAndExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  void EmitRISC(std::ostream& s, Context& c) const override;
  void Print(std::ostream& s) const override;
};

class BitXorExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  BitXorExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  void EmitRISC(std::ostream& s, Context& c) const override;
  void Print(std::ostream& s) const override;
};

class BitOrExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  BitOrExpression(NodePtr left, NodePtr right)
      : left_(std::move(left)), right_(std::move(right)) {}
  void EmitRISC(std::ostream& s, Context& c) const override;
  void Print(std::ostream& s) const override;
};

// unary

class NegExpression : public Node {
 private:
  NodePtr expr_;

 public:
  NegExpression(NodePtr expr) : expr_(std::move(expr)) {}
  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

}  // namespace ast
