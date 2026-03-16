#include "ast_logical.hpp"

#include <ostream>

#include "ast_context.hpp"

namespace ast {

// general helper func for preparing comparisons
// first emit left, push result, then emit right, pop left to t0

static void PrepareComparison(std::ostream& s, Context& c, const NodePtr& left,
                              const NodePtr& right) {
  left->EmitRISC(s, c);
  c.Push(s, "a0");
  right->EmitRISC(s, c);
  c.Pop(s, "t0");
}

static void PrintBinary(std::ostream& s, const NodePtr& left,
                        const NodePtr& right, const std::string& op_str) {
  s << "(";
  left->Print(s);
  s << " " << op_str << " ";
  right->Print(s);
  s << ")";
}

// equality

void EqualExpression::EmitRISC(std::ostream& s, Context& c) const {
  PrepareComparison(s, c, left_, right_);
  s << "  xor a0, t0, a0\n";  // same = 0
  s << "  seqz a0, a0\n";     // if = 0, a0 = 1
}

void NotEqualExpression::EmitRISC(std::ostream& s, Context& c) const {
  PrepareComparison(s, c, left_, right_);
  s << "  xor a0, t0, a0\n";  // different = 0
  s << "  snez a0, a0\n";     // if != 0, a0 = 1
}

// relational

void LessThanExpression::EmitRISC(std::ostream& s, Context& c) const {
  PrepareComparison(s, c, left_, right_);
  s << "  slt a0, t0, a0\n";  // a0 = (t0 < a0)
}

void GreaterThanExpression::EmitRISC(std::ostream& s, Context& c) const {
  PrepareComparison(s, c, left_, right_);
  s << "  slt a0, a0, t0\n";  // a > b = b < a
}

void LessEqualExpression::EmitRISC(std::ostream& s, Context& c) const {
  PrepareComparison(s, c, left_, right_);
  // a <= b = !(a > b) -> !(b < a)
  s << "  slt a0, a0, t0\n";
  s << "  xori a0, a0, 1\n";
}

void GreaterEqualExpression::EmitRISC(std::ostream& s, Context& c) const {
  PrepareComparison(s, c, left_, right_);
  // a >= b = !(a < b)
  s << "  slt a0, t0, a0\n";
  s << "  xori a0, a0, 1\n";
}

// logical
void LNotExpression::EmitRISC(std::ostream& s, Context& c) const {
  expr_->EmitRISC(s, c);
  s << "  seqz a0, a0\n";
}

void LAndExpression::EmitRISC(std::ostream& s, Context& c) const {
  std::string skip_label = c.CreateLabel("land", "_skip");
  std::string end_label = c.CreateLabel("land", "_end");

  left_->EmitRISC(s, c);
  s << "  beqz a0, " << skip_label << std::endl;  // jump when left is 0

  right_->EmitRISC(s, c);
  s << "  snez a0, a0\n";  // if right is not 0, a0 = 1, else 0
  s << "  j " << end_label << std::endl;

  s << skip_label << ":\n";
  s << "  li a0, 0\n";

  s << end_label << ":\n";
}

void LOrExpression::EmitRISC(std::ostream& s, Context& c) const {
  std::string hit_label = c.CreateLabel("lor", "_hit");
  std::string end_label = c.CreateLabel("lor", "_end");

  left_->EmitRISC(s, c);
  s << "  bnez a0, " << hit_label << std::endl;  // jump when left is not 0

  right_->EmitRISC(s, c);
  s << "  snez a0, a0\n";
  s << "  j " << end_label << std::endl;

  s << hit_label << ":\n";
  s << "  li a0, 1\n";

  s << end_label << ":\n";
}

// print

void EqualExpression::Print(std::ostream& s) const {
  PrintBinary(s, left_, right_, "==");
}
void NotEqualExpression::Print(std::ostream& s) const {
  PrintBinary(s, left_, right_, "!=");
}
void LessThanExpression::Print(std::ostream& s) const {
  PrintBinary(s, left_, right_, "<");
}
void GreaterThanExpression::Print(std::ostream& s) const {
  PrintBinary(s, left_, right_, ">");
}
void LessEqualExpression::Print(std::ostream& s) const {
  PrintBinary(s, left_, right_, "<=");
}
void GreaterEqualExpression::Print(std::ostream& s) const {
  PrintBinary(s, left_, right_, ">=");
}
void LAndExpression::Print(std::ostream& s) const {
  PrintBinary(s, left_, right_, "&&");
}
void LOrExpression::Print(std::ostream& s) const {
  PrintBinary(s, left_, right_, "||");
}

void LNotExpression::Print(std::ostream& s) const {
  s << "(!";
  expr_->Print(s);
  s << ")";
}

}  // namespace ast
