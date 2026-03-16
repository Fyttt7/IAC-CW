#pragma once
#include "ast_node.hpp"

namespace ast {
class PointerDeclarator : public Node {
 private:
  NodePtr base_;

 public:
  PointerDeclarator(NodePtr base) : base_(std::move(base)) {}
  void PreScan(Context& context) const override { base_->PreScan(context); }
  void EmitRISC(std::ostream& stream, Context& context) const override {
    base_->EmitRISC(stream, context);
  }
  void Print(std::ostream& stream) const override {
    stream << "*";
    base_->Print(stream);
  }
  std::string NameStr() const override { return base_->NameStr(); }
};
}  // namespace ast
