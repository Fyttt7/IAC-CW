#include "ast_direct_declarator.hpp"

#include "ast_identifier.hpp"

namespace ast {
int g_param_index = 0;

void DirectDeclarator::PreScan(Context& context) const {
  g_param_index = 0;
  if (parameters_) parameters_->PreScan(context);
}

void DirectDeclarator::EmitRISC(std::ostream& stream, Context& context) const {
  g_param_index = 0;
  if (parameters_) parameters_->EmitRISC(stream, context);
}

void DirectDeclarator::Print(std::ostream& stream) const {
  identifier_->Print(stream);
  if (is_array_) {
    stream << "[" << array_size_ << "]";
  } else {
    stream << "(";
    if (parameters_) parameters_->Print(stream);
    stream << ")";
  }
}

std::string DirectDeclarator::NameStr() const {
  return identifier_ ? identifier_->NameStr() : "";
}
}  // namespace ast
