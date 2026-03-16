#pragma once
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "ast_type_specifier.hpp"

namespace ast {

struct VarInfo {
  int offset;
  TypeSpecifier type;
  int size;
  bool is_global;
  bool is_pointer;
  bool is_array;
  std::string struct_name;
};

struct StructDef {
  int size = 0;
  std::map<std::string, int> member_offsets;
};

struct SwitchContext {
  std::vector<std::pair<int, std::string>> cases;
  std::string default_label = "";
};

class Context {
 private:
  int label_count_ = 0;
  std::string current_exit_label_;
  std::vector<std::map<std::string, VarInfo>> scopes_;
  std::map<std::string, StructDef> structs_;
  int current_stack_offset_;
  const VarInfo* Lookup(const std::string& name) const;
  std::vector<std::string> break_labels_;
  std::vector<SwitchContext> switch_contexts_;

 public:
  std::string CreateLabel(std::string prefix, std::string suffix);
  void Push(std::ostream& s, std::string reg);
  void Pop(std::ostream& s, std::string reg);
  void DefineStruct(const std::string& name, const StructDef& def);
  StructDef GetStruct(const std::string& name) const;
  void PushBreakLabel(std::string lbl) { break_labels_.push_back(lbl); }
  void PopBreakLabel() { break_labels_.pop_back(); }
  std::string GetBreakLabel() const { return break_labels_.back(); }

  void PushSwitchContext() { switch_contexts_.push_back(SwitchContext()); }
  void PopSwitchContext() { switch_contexts_.pop_back(); }
  void AddSwitchCase(int val, std::string lbl) {
    switch_contexts_.back().cases.push_back({val, lbl});
  }
  void SetSwitchDefault(std::string lbl) {
    switch_contexts_.back().default_label = lbl;
  }
  std::vector<std::pair<int, std::string>> GetCurrentSwitchCases() const {
    return switch_contexts_.back().cases;
  }
  std::string GetSwitchDefault() const {
    return switch_contexts_.back().default_label;
  }
  bool HasSwitchDefault() const {
    return switch_contexts_.back().default_label != "";
  }

  Context() : current_stack_offset_(-8) { EnterScope(); }

  void EnterScope();
  void ExitScope();

  bool IsGlobal() const { return scopes_.size() == 1; }

  void DeclareLocalVariable(const std::string& name, TypeSpecifier type,
                            int size, bool is_pointer = false,
                            bool is_array = false,
                            std::string struct_name = "");

  int GetVariableSize(const std::string& name) const;
  TypeSpecifier GetVariableType(const std::string& name) const;

  TypeSpecifier current_decl_type_ = TypeSpecifier::INT;
  void SetCurrentDeclType(TypeSpecifier t) { current_decl_type_ = t; }
  TypeSpecifier GetCurrentDeclType() const { return current_decl_type_; }

  bool IsPointer(const std::string& name) const;
  bool IsArray(const std::string& name) const;

  int GetVariableOffset(const std::string& name) const;
  bool IsVariableGlobal(const std::string& name) const;

  void IncrementStackSize(int size);
  void ResetStackOffset() { current_stack_offset_ = -8; }
  int GetTotalStackSize() const;

  void SetFunctionExitLabel(std::string label) { current_exit_label_ = label; }
  std::string GetFunctionExitLabel() const { return current_exit_label_; }

  std::string GetVariableStructName(const std::string& name) const;
};

}  // namespace ast
