#include "ast_pointer.hpp"

#include <stdexcept>

namespace ast {

void AddressOfExpression::PreScan(Context& context) const {
  operand_->PreScan(context);
}

void AddressOfExpression::EmitRISC(std::ostream& stream,
                                   Context& context) const {
  std::string name = operand_->NameStr();
  if (name.empty()) {
    throw std::runtime_error("Operand of & must be a variable");
  }

  if (context.IsVariableGlobal(name)) {
    stream << "    la a0, " << name << std::endl;
  } else {
    int offset = context.GetVariableOffset(name);
    stream << "    addi a0, s0, " << offset << std::endl;
  }
}

void AddressOfExpression::Print(std::ostream& stream) const {
  stream << "&";
  operand_->Print(stream);
}

void DereferenceExpression::PreScan(Context& context) const {
  operand_->PreScan(context);
}

void DereferenceExpression::EmitRISC(std::ostream& stream,
                                     Context& context) const {
  operand_->EmitRISC(stream, context);

  std::string base = operand_->GetBaseName();
  if (!base.empty() && context.GetVariableType(base) == TypeSpecifier::CHAR) {
    stream << "    lb a0, 0(a0)" << std::endl;
  } else {
    stream << "    lw a0, 0(a0)" << std::endl;
  }
}

void DereferenceExpression::Print(std::ostream& stream) const {
  stream << "*";
  operand_->Print(stream);
}

}  // namespace ast
