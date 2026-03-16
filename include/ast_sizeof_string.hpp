#pragma once
#include <string>

#include "ast_node.hpp"

namespace ast {

class SizeOfExpression : public Node {
 private:
  NodePtr operand_;

 public:
  SizeOfExpression(NodePtr operand) : operand_(std::move(operand)) {}
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& stream) const override;
};

class StringLiteral : public Node {
 private:
  std::string val_;

 public:
  StringLiteral(std::string val) : val_(std::move(val)) {}
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& stream) const override;
};

}  // namespace ast
