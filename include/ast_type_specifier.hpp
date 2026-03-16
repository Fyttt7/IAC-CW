#pragma once

#include <stdexcept>
#include <string_view>

namespace ast {

enum class TypeSpecifier { INT, CHAR, VOID, STRUCT, UNKNOWN };

template <typename LogStream>
LogStream& operator<<(LogStream& ls, const TypeSpecifier& type) {
  const auto TypeToString = [&type] {
    switch (type) {
      case TypeSpecifier::INT:
        return "int";
      case TypeSpecifier::CHAR:
        return "char";
      case TypeSpecifier::VOID:
        return "void";
      case TypeSpecifier::STRUCT:
        return "struct";
      case TypeSpecifier::UNKNOWN:
        return "unknown";
    }
    throw std::runtime_error("Unexpected type specifier");
  };
  return ls << TypeToString();
}

}  // namespace ast
