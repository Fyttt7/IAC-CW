#pragma once
#include "ast_node.hpp"

namespace ast {

class AddAssignExpression : public Node {
 private:
  NodePtr left_, right_;

 public:
  AddAssignExpression(NodePtr l, NodePtr r)
      : left_(std::move(l)), right_(std::move(r)) {}
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& /*stream*/) const override {}
};

class PostDecExpression : public Node {
 private:
  NodePtr operand_;

 public:
  PostDecExpression(NodePtr op) : operand_(std::move(op)) {}
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& /*stream*/) const override {}
};

}  // namespace ast
