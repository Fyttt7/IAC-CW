#include "ast_declaration.hpp"

#include "ast_direct_declarator.hpp"
#include "ast_identifier.hpp"
#include "ast_pointer_declarator.hpp"

namespace ast {

void Declaration::EmitRISC(std::ostream& stream, Context& context) const {
  context.SetCurrentDeclType(type_);
  if (declarators_ != nullptr) declarators_->EmitRISC(stream, context);
}

void Declaration::Print(std::ostream& stream) const {
  stream << "int ";
  if (declarators_ != nullptr) declarators_->Print(stream);
  stream << ";";
}

void Declaration::PreScan(Context& context) const {
  context.SetCurrentDeclType(type_);
  if (declarators_ != nullptr) declarators_->PreScan(context);
}

void InitDeclarator::PreScan(Context& context) const {
  bool is_function = false;
  if (auto dir_decl =
          dynamic_cast<const DirectDeclarator*>(declarator_.get())) {
    if (!dir_decl->IsArray()) is_function = true;
  }

  if (is_function) return;

  if (!context.IsGlobal()) {
    int size = (context.GetCurrentDeclType() == TypeSpecifier::CHAR) ? 1 : 4;
    if (auto dir_decl =
            dynamic_cast<const DirectDeclarator*>(declarator_.get())) {
      if (dir_decl->IsArray()) size = size * dir_decl->GetArraySize();
    }
    context.IncrementStackSize(size);
  }
}

void InitDeclarator::EmitRISC(std::ostream& stream, Context& context) const {
  bool is_function = false;
  if (auto dir_decl =
          dynamic_cast<const DirectDeclarator*>(declarator_.get())) {
    if (!dir_decl->IsArray()) is_function = true;
  }
  if (is_function) return;

  std::string name = GetIdentifierName();
  TypeSpecifier type = context.GetCurrentDeclType();

  int size = (type == TypeSpecifier::CHAR) ? 1 : 4;
  bool is_array = false;

  bool is_pointer =
      dynamic_cast<const PointerDeclarator*>(declarator_.get()) != nullptr;

  if (auto dir_decl =
          dynamic_cast<const DirectDeclarator*>(declarator_.get())) {
    if (dir_decl->IsArray()) {
      is_array = true;
      size = size * dir_decl->GetArraySize();
    }
  }

  context.DeclareLocalVariable(name, type, size, is_pointer, is_array);

  if (context.IsGlobal()) {
    stream << ".globl " << name << std::endl;
    stream << ".data" << std::endl;
    stream << ".align 2" << std::endl;
    stream << name << ":" << std::endl;
    if (is_array) {
      stream << "    .zero " << size << std::endl;
    } else {
      stream << "    .word 0" << std::endl;
    }
    stream << ".text" << std::endl;
  } else {
    if (initializer_ != nullptr) {
      initializer_->EmitRISC(stream, context);
      int offset = context.GetVariableOffset(name);
      stream << "    sw a0, " << offset << "(s0)" << std::endl;
    }
  }
}

std::string InitDeclarator::GetIdentifierName() const {
  return declarator_ ? declarator_->NameStr() : "";
}

void InitDeclarator::Print(std::ostream& stream) const {
  declarator_->Print(stream);
  if (initializer_ != nullptr) {
    stream << " = ";
    initializer_->Print(stream);
  }
}

}  // namespace ast
