#pragma once
#include "ast_node.hpp"

namespace ast {

class AddressOfExpression : public Node {
 private:
  NodePtr operand_;

 public:
  AddressOfExpression(NodePtr operand) : operand_(std::move(operand)) {}
  void PreScan(Context& context) const override;
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& stream) const override;
};

class DereferenceExpression : public Node {
 private:
  NodePtr operand_;

 public:
  DereferenceExpression(NodePtr operand) : operand_(std::move(operand)) {}

  void PreScan(Context& context) const override;
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& stream) const override;

  const NodePtr& GetOperand() const { return operand_; }
};

}  // namespace ast
