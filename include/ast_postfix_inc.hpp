#pragma once
#include "ast_node.hpp"

namespace ast {
class PostfixInc : public Node {
 private:
  NodePtr expr_;

 public:
  PostfixInc(NodePtr expr) : expr_(std::move(expr)) {}

  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& stream) const override;
  void PreScan(Context& context) const override;
};
}  // namespace ast
