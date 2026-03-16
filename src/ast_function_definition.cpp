#include "ast_function_definition.hpp"

#include "ast_direct_declarator.hpp"

namespace ast {

void FunctionDefinition::EmitRISC(std::ostream& stream,
                                  Context& context) const {
  // prescan
  context.ResetStackOffset();
  context.EnterScope();
  declarator_->PreScan(context);

  if (compound_statement_) {
    compound_statement_->PreScan(context);
  }
  int stack_size = context.GetTotalStackSize();
  context.ExitScope();

  // code gen
  context.ResetStackOffset();
  std::string func_name = declarator_->NameStr();
  std::string exit_label = context.CreateLabel(func_name, "_ret_targ");
  context.SetFunctionExitLabel(exit_label);

  stream << ".text" << std::endl;
  stream << ".globl " << func_name << std::endl;
  stream << func_name << ":" << std::endl;

  // prologue
  stream << "    addi  sp, sp, -" << stack_size << std::endl;
  stream << "    sw    ra, " << stack_size - 4 << "(sp)" << std::endl;
  stream << "    sw    s0, " << stack_size - 8 << "(sp)" << std::endl;
  stream << "    addi  s0, sp, " << stack_size << std::endl;

  context.EnterScope();
  declarator_->EmitRISC(stream, context);

  if (compound_statement_ != nullptr) {
    compound_statement_->EmitRISC(stream, context);
  }
  context.ExitScope();

  // epilogue
  stream << exit_label << ":" << std::endl;
  stream << "    lw    s0, " << stack_size - 8 << "(sp)" << std::endl;
  stream << "    lw    ra, " << stack_size - 4 << "(sp)" << std::endl;
  stream << "    addi  sp, sp, " << stack_size << std::endl;
  stream << "    ret" << std::endl;
}

void FunctionDefinition::Print(std::ostream& stream) const {
  stream << declaration_specifiers_ << " ";
  declarator_->Print(stream);
  if (compound_statement_ != nullptr) {
    compound_statement_->Print(stream);
  }
}
}  // namespace ast
