#include "ast_sizeof_string.hpp"

namespace ast {

void SizeOfExpression::EmitRISC(std::ostream& stream, Context& context) const {
  std::string name = operand_->NameStr();
  int size = 4;
  if (!name.empty()) {
    try {
      size = context.GetVariableSize(name);
    } catch (...) {
    }
  }
  stream << "    li a0, " << size << std::endl;
}

void SizeOfExpression::Print(std::ostream& stream) const {
  stream << "sizeof(";
  operand_->Print(stream);
  stream << ")";
}

void StringLiteral::EmitRISC(std::ostream& stream, Context& context) const {
  std::string label = context.CreateLabel("str", "");
  stream << "    .section .rodata" << std::endl;
  stream << label << ":" << std::endl;
  stream << "    .string " << val_ << std::endl;
  stream << "    .text" << std::endl;
  stream << "    la a0, " << label << std::endl;
}

void StringLiteral::Print(std::ostream& stream) const { stream << val_; }

}  // namespace ast
