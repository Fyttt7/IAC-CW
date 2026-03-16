#pragma once
#include <string>

#include "ast_node.hpp"

namespace ast {

class StructMemberDecl : public Node {
 public:
  TypeSpecifier type_;
  std::string name_;
  StructMemberDecl(TypeSpecifier type, std::string name)
      : type_(type), name_(std::move(name)) {}

  void EmitRISC(std::ostream& /* stream */,
                Context& /* context */) const override {}
  void Print(std::ostream& /* stream */) const override {}
};

class StructDefinition : public Node {
 private:
  std::string name_;
  NodePtr member_list_;

 public:
  StructDefinition(std::string name, NodePtr member_list)
      : name_(std::move(name)), member_list_(std::move(member_list)) {}

  void PreScan(Context& context) const override;
  void EmitRISC(std::ostream& /* stream */, Context& context) const override {
    PreScan(context);
  }
  void Print(std::ostream& /* stream */) const override {}
};

class StructVarDeclaration : public Node {
 private:
  std::string struct_name_;
  std::string var_name_;

 public:
  StructVarDeclaration(std::string struct_name, std::string var_name)
      : struct_name_(std::move(struct_name)), var_name_(std::move(var_name)) {}

  void PreScan(Context& context) const override;
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& /* stream */) const override {}
};

}  // namespace ast
