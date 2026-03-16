#pragma once
#include "ast_node.hpp"

namespace ast {
class DirectDeclarator : public Node {
 private:
  NodePtr identifier_;
  NodePtr parameters_;
  bool is_array_ = false;
  int array_size_ = 0;

 public:
  DirectDeclarator(NodePtr identifier, NodePtr parameters = nullptr)
      : identifier_(std::move(identifier)),
        parameters_(std::move(parameters)) {}

  DirectDeclarator(NodePtr identifier, int array_size)
      : identifier_(std::move(identifier)),
        is_array_(true),
        array_size_(array_size) {}

  void PreScan(Context& context) const override;
  void EmitRISC(std::ostream& stream, Context& context) const override;
  void Print(std::ostream& stream) const override;
  virtual std::string NameStr() const override;

  bool IsArray() const { return is_array_; }
  int GetArraySize() const { return array_size_; }
};
}  // namespace ast
