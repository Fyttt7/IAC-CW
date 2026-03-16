#include "ast_parameter.hpp"

#include "ast_pointer_declarator.hpp"

namespace ast {
extern int g_param_index;

void Parameter::PreScan(Context& context) const {
  bool is_ptr =
      dynamic_cast<const PointerDeclarator*>(declarator_.get()) != nullptr;
  context.DeclareLocalVariable(declarator_->NameStr(), type_, 4, is_ptr);
}

void Parameter::EmitRISC(std::ostream& stream, Context& context) const {
  bool is_ptr =
      dynamic_cast<const PointerDeclarator*>(declarator_.get()) != nullptr;
  context.DeclareLocalVariable(declarator_->NameStr(), type_, 4, is_ptr);

  int offset = context.GetVariableOffset(declarator_->NameStr());
  stream << "    sw a" << g_param_index << ", " << offset << "(s0)"
         << std::endl;
  g_param_index++;
}

void Parameter::Print(std::ostream& stream) const {
  stream << type_ << " ";
  declarator_->Print(stream);
}
}  // namespace ast
