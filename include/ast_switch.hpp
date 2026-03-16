#pragma once
#include <string>

#include "ast_node.hpp"

namespace ast {

class SwitchStatement : public Node {
 private:
  NodePtr expr_;
  NodePtr body_;

 public:
  SwitchStatement(NodePtr expr, NodePtr body)
      : expr_(std::move(expr)), body_(std::move(body)) {}
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& /* stream */) const override {}
};

class CaseStatement : public Node {
 private:
  int value_;
  NodePtr stmt_;

 public:
  CaseStatement(int val, NodePtr stmt) : value_(val), stmt_(std::move(stmt)) {}
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& /* stream */) const override {}
};

class DefaultStatement : public Node {
 private:
  NodePtr stmt_;

 public:
  DefaultStatement(NodePtr stmt) : stmt_(std::move(stmt)) {}
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& /* stream */) const override {}
};

class BreakStatement : public Node {
 public:
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& /* stream */) const override {}
};

}  // namespace ast
