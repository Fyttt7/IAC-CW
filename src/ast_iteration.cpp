#include "ast_iteration.hpp"

#include <ostream>

#include "ast_context.hpp"

namespace ast {

void WhileStatement::EmitRISC(std::ostream& stream, Context& context) const {
  std::string start_label = context.CreateLabel("while", "_start");
  std::string end_label = context.CreateLabel("while", "_end");

  stream << start_label << ":" << std::endl;

  condition_->EmitRISC(stream, context);

  // a0=0, skip
  stream << "    beqz  a0, " << end_label << std::endl;

  if (body_ != nullptr) {
    body_->EmitRISC(stream, context);
  }

  stream << "    j     " << start_label << std::endl;

  stream << end_label << ":" << std::endl;
}

void WhileStatement::Print(std::ostream& stream) const {
  stream << "while (";
  condition_->Print(stream);
  stream << ") ";
  if (body_ != nullptr) {
    body_->Print(stream);
  }
}

void WhileStatement::PreScan(Context& context) const {
  if (condition_) condition_->PreScan(context);
  if (body_) body_->PreScan(context);
}

void ForStatement::EmitRISC(std::ostream& stream, Context& context) const {
  std::string start_label = context.CreateLabel("for", "_start");
  std::string end_label = context.CreateLabel("for", "_end");

  if (init_ != nullptr) init_->EmitRISC(stream, context);

  stream << start_label << ":" << std::endl;

  if (condition_ != nullptr) {
    condition_->EmitRISC(stream, context);
    stream << "    beqz  a0, " << end_label << std::endl;
  }

  if (body_ != nullptr) body_->EmitRISC(stream, context);

  if (update_ != nullptr) update_->EmitRISC(stream, context);

  stream << "    j     " << start_label << std::endl;

  stream << end_label << ":" << std::endl;
}

void ForStatement::Print(std::ostream& stream) const {
  stream << "for (";
  if (init_) init_->Print(stream);
  stream << "; ";
  if (condition_) condition_->Print(stream);
  stream << "; ";
  if (update_) update_->Print(stream);
  stream << ") ";
  if (body_) body_->Print(stream);
}

void ForStatement::PreScan(Context& context) const {
  if (init_) init_->PreScan(context);
  if (condition_) condition_->PreScan(context);
  if (update_) update_->PreScan(context);
  if (body_) body_->PreScan(context);
}

}  // namespace ast
