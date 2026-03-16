#include "ast_selection.hpp"

#include <ostream>

#include "ast_context.hpp"

namespace ast {

void IfStatement::EmitRISC(std::ostream& stream, Context& context) const {
  std::string else_label = context.CreateLabel("if", "_else");
  std::string end_label = context.CreateLabel("if", "_end");

  condition_->EmitRISC(stream, context);

  // a0=0, else or end
  if (else_branch_ != nullptr) {
    stream << "    beqz  a0, " << else_label << std::endl;
  } else {
    stream << "    beqz  a0, " << end_label << std::endl;
  }

  if (then_branch_ != nullptr) {
    then_branch_->EmitRISC(stream, context);
  }

  // skip else if exists
  if (else_branch_ != nullptr) {
    stream << "    j     " << end_label << std::endl;

    stream << else_label << ":" << std::endl;
    else_branch_->EmitRISC(stream, context);
  }

  stream << end_label << ":" << std::endl;
}

void IfStatement::Print(std::ostream& stream) const {
  stream << "if (";
  condition_->Print(stream);
  stream << ") ";
  then_branch_->Print(stream);
  if (else_branch_ != nullptr) {
    stream << " else ";
    else_branch_->Print(stream);
  }
}

void IfStatement::PreScan(Context& context) const {
  if (condition_) condition_->PreScan(context);
  if (then_branch_) then_branch_->PreScan(context);
  if (else_branch_) else_branch_->PreScan(context);
}

}  // namespace ast
