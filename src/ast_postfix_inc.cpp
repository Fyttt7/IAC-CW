#include "ast_postfix_inc.hpp"

#include <stdexcept>

namespace ast {

void PostfixInc::PreScan(Context& context) const { expr_->PreScan(context); }

void PostfixInc::EmitRISC(std::ostream& stream, Context& context) const {
  expr_->EmitRISC(stream, context);

  std::string name = expr_->NameStr();
  if (name.empty()) {
    throw std::runtime_error(
        "Postfix ++ only supports simple variables right now");
  }
  int offset = context.GetVariableOffset(name);

  stream << "    addi t0, a0, 1" << std::endl;

  stream << "    sw t0, " << offset << "(s0)" << std::endl;
}

void PostfixInc::Print(std::ostream& stream) const {
  expr_->Print(stream);
  stream << "++";
}

}  // namespace ast
