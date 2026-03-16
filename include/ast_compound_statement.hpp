#pragma once

#include "ast_node.hpp"

namespace ast {

class CompoundStatement : public Node {
 private:
  NodePtr declaration_list_;
  NodePtr statement_list_;

 public:
  CompoundStatement(NodePtr declaration_list, NodePtr statement_list)
      : declaration_list_(std::move(declaration_list)),
        statement_list_(std::move(statement_list)) {}

  virtual void PreScan(Context& context) const override;

  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
};

}  // namespace ast
