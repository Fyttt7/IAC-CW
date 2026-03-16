#include "ast_jump_statement.hpp"

namespace ast {

void ReturnStatement::EmitRISC(std::ostream& stream, Context& context) const {
  if (expression_ != nullptr) {
    expression_->EmitRISC(stream, context);
  }
  stream << "    j " << context.GetFunctionExitLabel() << std::endl;
}

void ReturnStatement::Print(std::ostream& stream) const {
  stream << "return";
  if (expression_ != nullptr) {
    stream << " ";
    expression_->Print(stream);
  }
  stream << ";" << std::endl;
}

}  // namespace ast
