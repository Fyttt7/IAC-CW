#pragma once

#include <string>

#include "ast_node.hpp"
#include "ast_type_specifier.hpp"

namespace ast {

// handle declarations of var
class Declaration : public Node {
 private:
  TypeSpecifier type_;
  NodePtr declarators_;

 public:
  Declaration(TypeSpecifier type, NodePtr declarators)
      : type_(type), declarators_(std::move(declarators)) {}

  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
  virtual void PreScan(Context& context) const override;
};

// initialize with value
class InitDeclarator : public Node {
 private:
  NodePtr declarator_;
  NodePtr initializer_;

 public:
  InitDeclarator(NodePtr declarator, NodePtr initializer)
      : declarator_(std::move(declarator)),
        initializer_(std::move(initializer)) {}

  virtual void EmitRISC(std::ostream& stream, Context& context) const override;
  virtual void Print(std::ostream& stream) const override;
  virtual void PreScan(Context& context) const override;

  std::string GetIdentifierName() const;
};

}  // namespace ast
