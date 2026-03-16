#include "ast_function_call.hpp"

namespace ast {

void FunctionCall::PreScan(Context& context) const {
  target_->PreScan(context);
  if (arguments_ != nullptr) {
    arguments_->PreScan(context);
  }
}

void FunctionCall::EmitRISC(std::ostream& stream, Context& context) const {
  std::string func_name = target_->NameStr();
  int arg_count = 0;

  if (arguments_ != nullptr) {
    const NodeList* arg_list = dynamic_cast<const NodeList*>(arguments_.get());
    if (arg_list) {
      const auto& nodes = arg_list->GetNodes();
      arg_count = nodes.size();

      for (const auto& arg : nodes) {
        arg->EmitRISC(stream, context);
        context.Push(stream, "a0");
      }

      for (int i = arg_count - 1; i >= 0; --i) {
        context.Pop(stream, "a" + std::to_string(i));
      }
    }
  }

  stream << "    call " << func_name << std::endl;
}

void FunctionCall::Print(std::ostream& stream) const {
  target_->Print(stream);
  stream << "(";
  if (arguments_ != nullptr) {
    arguments_->Print(stream);
  }
  stream << ")";
}

}  // namespace ast
