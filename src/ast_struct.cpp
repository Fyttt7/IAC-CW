#include "ast_struct.hpp"

namespace ast {

void StructDefinition::PreScan(Context& context) const {
  StructDef def;
  int current_offset = 0;

  if (member_list_ != nullptr) {
    const NodeList* list = dynamic_cast<const NodeList*>(member_list_.get());
    if (list) {
      for (const auto& node : list->GetNodes()) {
        const StructMemberDecl* mem =
            dynamic_cast<const StructMemberDecl*>(node.get());
        if (mem) {
          def.member_offsets[mem->name_] = current_offset;
          current_offset += (mem->type_ == TypeSpecifier::CHAR) ? 1 : 4;
        }
      }
    }
  }

  int remainder = current_offset % 4;
  if (remainder != 0) current_offset += (4 - remainder);
  def.size = current_offset;

  context.DefineStruct(name_, def);
}

void StructVarDeclaration::PreScan(Context& context) const {
  if (!context.IsGlobal()) {
    StructDef def = context.GetStruct(struct_name_);
    context.IncrementStackSize(def.size);
  }
}

void StructVarDeclaration::EmitRISC(std::ostream& stream,
                                    Context& context) const {
  StructDef def = context.GetStruct(struct_name_);
  std::string name = var_name_;

  context.DeclareLocalVariable(name, TypeSpecifier::STRUCT, def.size, false,
                               false, struct_name_);

  if (context.IsGlobal()) {
    stream << ".globl " << name << std::endl;
    stream << ".data" << std::endl;
    stream << ".align 2" << std::endl;
    stream << name << ":" << std::endl;
    stream << "    .zero " << def.size << std::endl;
    stream << ".text" << std::endl;
  }
}

}  // namespace ast
