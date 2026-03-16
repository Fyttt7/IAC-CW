#include "ast_switch.hpp"

namespace ast {

void SwitchStatement::EmitRISC(std::ostream& stream, Context& context) const {
  expr_->EmitRISC(stream, context);
  context.Push(stream, "a0");

  std::string test_lbl = context.CreateLabel("switch_test", "");
  std::string exit_lbl = context.CreateLabel("switch_exit", "");

  context.PushSwitchContext();
  context.PushBreakLabel(exit_lbl);

  stream << "    j " << test_lbl << std::endl;

  body_->EmitRISC(stream, context);
  stream << "    j " << exit_lbl << std::endl;

  stream << test_lbl << ":" << std::endl;

  context.Pop(stream, "t0");

  for (auto c : context.GetCurrentSwitchCases()) {
    stream << "    li t1, " << c.first << std::endl;
    stream << "    beq t0, t1, " << c.second << std::endl;
  }

  if (context.HasSwitchDefault()) {
    stream << "    j " << context.GetSwitchDefault() << std::endl;
  } else {
    stream << "    j " << exit_lbl << std::endl;
  }

  stream << exit_lbl << ":" << std::endl;

  context.PopBreakLabel();
  context.PopSwitchContext();
}

void CaseStatement::EmitRISC(std::ostream& stream, Context& context) const {
  std::string lbl = context.CreateLabel("case", "");
  context.AddSwitchCase(value_, lbl);
  stream << lbl << ":" << std::endl;
  stmt_->EmitRISC(stream, context);
}

void DefaultStatement::EmitRISC(std::ostream& stream, Context& context) const {
  std::string lbl = context.CreateLabel("default", "");
  context.SetSwitchDefault(lbl);
  stream << lbl << ":" << std::endl;
  stmt_->EmitRISC(stream, context);
}

void BreakStatement::EmitRISC(std::ostream& stream, Context& context) const {
  std::string exit_lbl = context.GetBreakLabel();
  stream << "    j " << exit_lbl << std::endl;
}

}  // namespace ast
