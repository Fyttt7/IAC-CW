#include "ast_identifier.hpp"

namespace ast {

void Identifier::EmitRISC(std::ostream& stream, Context& context) const {
  if (context.IsArray(identifier_)) {
    if (context.IsVariableGlobal(identifier_)) {
      stream << "    la a0, " << identifier_ << std::endl;
    } else {
      int offset = context.GetVariableOffset(identifier_);
      stream << "    addi a0, s0, " << offset << std::endl;
    }
  } else if (context.IsVariableGlobal(identifier_)) {
    stream << "    lui a0, %hi(" << identifier_ << ")" << std::endl;
    stream << "    lw a0, %lo(" << identifier_ << ")(a0)" << std::endl;
  } else {
    int offset = context.GetVariableOffset(identifier_);
    stream << "    lw a0, " << offset << "(s0)" << std::endl;
  }
}

void Identifier::Print(std::ostream& stream) const { stream << identifier_; }
}  // namespace ast
