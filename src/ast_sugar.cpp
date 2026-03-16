#include "ast_sugar.hpp"

namespace ast {

void AddAssignExpression::EmitRISC(std::ostream& stream,
                                   Context& context) const {
  right_->EmitRISC(stream, context);
  context.Push(stream, "a0");
  left_->EmitRISC(stream, context);
  stream << "    mv t1, a0" << std::endl;
  context.Pop(stream, "a0");
  stream << "    add a0, t1, a0" << std::endl;

  int offset = context.GetVariableOffset(left_->NameStr());
  stream << "    sw a0, " << offset << "(s0)" << std::endl;
}

void PostDecExpression::EmitRISC(std::ostream& stream, Context& context) const {
  operand_->EmitRISC(stream, context);
  stream << "    addi t1, a0, -1" << std::endl;

  int offset = context.GetVariableOffset(operand_->NameStr());
  stream << "    sw t1, " << offset << "(s0)" << std::endl;
}

}  // namespace ast
