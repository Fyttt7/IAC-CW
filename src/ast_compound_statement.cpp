#include "ast_compound_statement.hpp"

namespace ast {

void CompoundStatement::EmitRISC(std::ostream& stream, Context& context) const {
  context.EnterScope();

  // declare
  if (declaration_list_ != nullptr) {
    declaration_list_->EmitRISC(stream, context);
  }

  // initialize
  if (statement_list_ != nullptr) {
    statement_list_->EmitRISC(stream, context);
  }

  context.ExitScope();
}

void CompoundStatement::Print(std::ostream& stream) const {
  stream << "{" << std::endl;
  if (declaration_list_ != nullptr) declaration_list_->Print(stream);
  if (statement_list_ != nullptr) statement_list_->Print(stream);
  stream << "}" << std::endl;
}

void CompoundStatement::PreScan(Context& context) const {
  context.EnterScope();
  // scan in declaration list to declare variables before scanning statements
  if (declaration_list_ != nullptr) {
    declaration_list_->PreScan(context);
  }
  // scan in statement list to handle nested compound statements
  if (statement_list_ != nullptr) {
    statement_list_->PreScan(context);
  }
  context.ExitScope();
}

}  // namespace ast
