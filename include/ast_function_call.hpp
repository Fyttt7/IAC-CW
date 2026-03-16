#pragma once
#include "ast_node.hpp"

namespace ast {

class FunctionCall : public Node {
 private:
  NodePtr target_;
  NodePtr arguments_;

 public:
  FunctionCall(NodePtr target, NodePtr arguments = nullptr)
      : target_(std::move(target)), arguments_(std::move(arguments)) {}

  void PreScan(Context& context) const override;
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& stream) const override;
};

}  // namespace ast
