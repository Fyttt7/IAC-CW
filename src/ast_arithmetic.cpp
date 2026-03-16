#include "ast_arithmetic.hpp"

#include "ast_context.hpp"
#include "ast_identifier.hpp"

namespace ast {

// general helper func for binary expressions
// first emit left, push result, then emit right, pop left to t0, and finally do
// the op
// since all bin expression share the same pattern
static void EmitBinary(std::ostream& stream, Context& context,
                       const NodePtr& left_, const NodePtr& right_,
                       const std::string& op) {
  left_->EmitRISC(stream, context);
  context.Push(stream, "a0");

  right_->EmitRISC(stream, context);

  if (op == "add" || op == "sub") {
    std::string base = left_->GetBaseName();
    if (!base.empty()) {
      if (context.IsPointer(base) || context.IsArray(base)) {
        if (context.GetVariableType(base) == TypeSpecifier::INT) {
          stream << "  slli a0, a0, 2" << std::endl;
        }
      }
    }
  }

  context.Pop(stream, "t0");

  stream << "  " << op << " a0, t0, a0" << std::endl;
}

static void PrintBinary(std::ostream& stream, const NodePtr& left_,
                        const NodePtr& right_, const std::string& op_str) {
  stream << "(";
  left_->Print(stream);
  stream << " " << op_str << " ";
  right_->Print(stream);
  stream << ")";
}

// arithmetic

void AddExpression::EmitRISC(std::ostream& stream, Context& context) const {
  EmitBinary(stream, context, left_, right_, "add");
}
void AddExpression::Print(std::ostream& stream) const {
  PrintBinary(stream, left_, right_, "+");
}

void SubExpression::EmitRISC(std::ostream& stream, Context& context) const {
  EmitBinary(stream, context, left_, right_, "sub");
}
void SubExpression::Print(std::ostream& stream) const {
  PrintBinary(stream, left_, right_, "-");
}

void MulExpression::EmitRISC(std::ostream& stream, Context& context) const {
  EmitBinary(stream, context, left_, right_, "mul");
}
void MulExpression::Print(std::ostream& stream) const {
  PrintBinary(stream, left_, right_, "*");
}

void DivExpression::EmitRISC(std::ostream& stream, Context& context) const {
  EmitBinary(stream, context, left_, right_, "div");
}
void DivExpression::Print(std::ostream& stream) const {
  PrintBinary(stream, left_, right_, "/");
}

void ModExpression::EmitRISC(std::ostream& stream, Context& context) const {
  EmitBinary(stream, context, left_, right_, "rem");
}
void ModExpression::Print(std::ostream& stream) const {
  PrintBinary(stream, left_, right_, "%");
}

// bitwise

void ShiftLeftExpression::EmitRISC(std::ostream& stream,
                                   Context& context) const {
  EmitBinary(stream, context, left_, right_, "sll");
}
void ShiftLeftExpression::Print(std::ostream& stream) const {
  PrintBinary(stream, left_, right_, "<<");
}

void ShiftRightExpression::EmitRISC(std::ostream& stream,
                                    Context& context) const {
  EmitBinary(stream, context, left_, right_, "sra");
}
void ShiftRightExpression::Print(std::ostream& stream) const {
  PrintBinary(stream, left_, right_, ">>");
}

void BitAndExpression::EmitRISC(std::ostream& stream, Context& context) const {
  EmitBinary(stream, context, left_, right_, "and");
}
void BitAndExpression::Print(std::ostream& stream) const {
  PrintBinary(stream, left_, right_, "&");
}

void BitXorExpression::EmitRISC(std::ostream& stream, Context& context) const {
  EmitBinary(stream, context, left_, right_, "xor");
}
void BitXorExpression::Print(std::ostream& stream) const {
  PrintBinary(stream, left_, right_, "^");
}

void BitOrExpression::EmitRISC(std::ostream& stream, Context& context) const {
  EmitBinary(stream, context, left_, right_, "or");
}
void BitOrExpression::Print(std::ostream& stream) const {
  PrintBinary(stream, left_, right_, "|");
}

// unary
// special case for NegExp

void NegExpression::EmitRISC(std::ostream& stream, Context& context) const {
  expr_->EmitRISC(stream, context);
  stream << "  neg a0, a0" << std::endl;
}

void NegExpression::Print(std::ostream& stream) const {
  stream << "(-";
  expr_->Print(stream);
  stream << ")";
}

}  // namespace ast
