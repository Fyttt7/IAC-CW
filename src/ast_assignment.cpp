#include "ast_assignment.hpp"

#include "ast_pointer.hpp"
#include "ast_struct_access.hpp"

namespace ast {

void AssignmentExpression::EmitRISC(std::ostream& stream,
                                    Context& context) const {
  right_->EmitRISC(stream, context);

  auto struct_access = dynamic_cast<const StructMemberAccess*>(left_.get());
  if (struct_access) {
    context.Push(stream, "a0");
    std::string var_name = struct_access->GetVarName();
    if (context.IsVariableGlobal(var_name)) {
      int mem_offset = struct_access->GetMemberOffset(context);
      stream << "    la t1, " << var_name << std::endl;
      context.Pop(stream, "a0");
      stream << "    sw a0, " << mem_offset << "(t1)" << std::endl;
    } else {
      int total_offset = struct_access->GetTotalOffset(context);
      context.Pop(stream, "a0");
      stream << "    sw a0, " << total_offset << "(s0)" << std::endl;
    }
    return;
  }

  auto deref = dynamic_cast<const DereferenceExpression*>(left_.get());
  if (deref) {
    context.Push(stream, "a0");
    deref->GetOperand()->EmitRISC(stream, context);
    stream << "    mv t1, a0" << std::endl;
    context.Pop(stream, "a0");

    std::string base = deref->GetOperand()->GetBaseName();
    if (!base.empty() && context.GetVariableType(base) == TypeSpecifier::CHAR) {
      stream << "    sb a0, 0(t1)" << std::endl;
    } else {
      stream << "    sw a0, 0(t1)" << std::endl;
    }
  } else {
    std::string name = left_->NameStr();
    int offset = context.GetVariableOffset(name);
    if (context.IsVariableGlobal(name)) {
    } else {
      stream << "    sw a0, " << offset << "(s0)" << std::endl;
    }
  }
}

void AssignmentExpression::Print(std::ostream& stream) const {
  left_->Print(stream);
  stream << " = ";
  right_->Print(stream);
}

}  // namespace ast
