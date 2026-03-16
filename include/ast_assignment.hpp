#pragma once

#include "ast_node.hpp"

namespace ast {

class AssignmentExpression : public Node {
 private:
  NodePtr left_;
  NodePtr right_;

 public:
  AssignmentExpression(NodePtr lhs, NodePtr rhs)
      : left_(std::move(lhs)), right_(std::move(rhs)) {}

  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

}  // namespace ast
