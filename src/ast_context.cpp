#include "ast_context.hpp"

#include <ostream>

#include "ast_type_specifier.hpp"

namespace ast {

std::string Context::CreateLabel(std::string prefix, std::string suffix) {
  std::string label = "." + prefix + suffix + std::to_string(label_count_);
  ++label_count_;
  return label;
}

void Context::Push(std::ostream& stream, std::string reg) {
  stream << "  addi sp, sp, -4" << std::endl;
  stream << "  sw " << reg << ", 0(sp)" << std::endl;
}

void Context::Pop(std::ostream& stream, std::string reg) {
  stream << "  lw " << reg << ", 0(sp)" << std::endl;
  stream << "  addi sp, sp, 4" << std::endl;
}

void Context::EnterScope() { scopes_.emplace_back(); }

void Context::ExitScope() {
  if (scopes_.size() <= 1) {
    throw std::runtime_error("Cannot exit global scope");
  }
  scopes_.pop_back();
}

void Context::DeclareLocalVariable(const std::string& name, TypeSpecifier type,
                                   int size, bool is_pointer, bool is_array,
                                   std::string struct_name) {
  if (scopes_.back().count(name))
    throw std::runtime_error("Variable already exist: " + name);
  bool is_glob = IsGlobal();
  int offset = 0;
  if (!is_glob) {
    current_stack_offset_ -= size;
    int remainder = (-current_stack_offset_) % 4;
    if (remainder != 0) current_stack_offset_ -= (4 - remainder);
    offset = current_stack_offset_;
  }

  VarInfo var{offset, type, size, is_glob, is_pointer, is_array, struct_name};
  scopes_.back().emplace(name, var);
}

void Context::IncrementStackSize(int size) { current_stack_offset_ -= size; }

int Context::GetVariableOffset(const std::string& name) const {
  auto var = Lookup(name);
  if (!var) throw std::runtime_error("Undefined variable: " + name);
  return var->offset;
}

bool Context::IsVariableGlobal(const std::string& name) const {
  auto var = Lookup(name);
  if (!var) throw std::runtime_error("Undefined variable: " + name);
  return var->is_global;
}

bool Context::IsPointer(const std::string& name) const {
  auto var = Lookup(name);
  if (!var) throw std::runtime_error("Undefined variable: " + name);
  return var->is_pointer;
}

bool Context::IsArray(const std::string& name) const {
  auto var = Lookup(name);
  if (!var) throw std::runtime_error("Undefined variable: " + name);
  return var->is_array;
}

int Context::GetTotalStackSize() const {
  int size = -current_stack_offset_;
  int remainder = size % 16;
  if (remainder > 0) size += (16 - remainder);
  return size;
}

const VarInfo* Context::Lookup(const std::string& name) const {
  for (int i = scopes_.size() - 1; i >= 0; --i) {
    auto it = scopes_[i].find(name);
    if (it != scopes_[i].end()) return &it->second;
  }
  return nullptr;
}

int Context::GetVariableSize(const std::string& name) const {
  auto var = Lookup(name);
  if (!var) throw std::runtime_error("Undefined variable: " + name);
  return var->size;
}

TypeSpecifier Context::GetVariableType(const std::string& name) const {
  auto var = Lookup(name);
  if (!var) throw std::runtime_error("Undefined variable: " + name);
  return var->type;
}

void Context::DefineStruct(const std::string& name, const StructDef& def) {
  structs_[name] = def;
}

StructDef Context::GetStruct(const std::string& name) const {
  if (structs_.find(name) == structs_.end()) {
    throw std::runtime_error("Undefined struct: " + name);
  }
  return structs_.at(name);
}

std::string Context::GetVariableStructName(const std::string& name) const {
  auto var = Lookup(name);
  if (!var) throw std::runtime_error("Undefined variable: " + name);
  return var->struct_name;
}

}  // namespace ast
