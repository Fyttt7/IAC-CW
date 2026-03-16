#pragma once
#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {
class Parameter : public Node {
 private:
  TypeSpecifier type_;
  NodePtr declarator_;

 public:
  Parameter(TypeSpecifier type, NodePtr declarator)
      : type_(type), declarator_(std::move(declarator)) {}

  void PreScan(Context& context) const override;
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& stream) const override;
};
}  // namespace ast
