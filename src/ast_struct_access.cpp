#include "ast_struct_access.hpp"

namespace ast {

void StructMemberAccess::EmitRISC(std::ostream& stream,
                                  Context& context) const {
  std::string var_name = GetVarName();
  if (context.IsVariableGlobal(var_name)) {
    int mem_offset = GetMemberOffset(context);
    stream << "    la t1, " << var_name << std::endl;
    stream << "    lw a0, " << mem_offset << "(t1)" << std::endl;
  } else {
    int total = GetTotalOffset(context);
    stream << "    lw a0, " << total << "(s0)" << std::endl;
  }
}

}  // namespace ast
