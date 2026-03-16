#pragma once
#include <string>

#include "ast_node.hpp"

namespace ast {

class StructMemberAccess : public Node {
 private:
  NodePtr struct_var_;
  std::string member_name_;

 public:
  StructMemberAccess(NodePtr var, std::string member)
      : struct_var_(std::move(var)), member_name_(std::move(member)) {}

  std::string GetVarName() const { return struct_var_->NameStr(); }

  int GetMemberOffset(Context& context) const {
    std::string var_name = GetVarName();
    std::string struct_name = context.GetVariableStructName(var_name);
    StructDef def = context.GetStruct(struct_name);
    return def.member_offsets.at(member_name_);
  }

  int GetTotalOffset(Context& context) const {
    std::string var_name = GetVarName();
    int base_offset = context.GetVariableOffset(var_name);
    return base_offset + GetMemberOffset(context);
  }

  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& /* stream */) const override {}
};

}  // namespace ast
