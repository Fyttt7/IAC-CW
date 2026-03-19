#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "ast_arithmetic.hpp"
#include "ast_assignment.hpp"
#include "ast_compound_statement.hpp"
#include "ast_constant.hpp"
#include "ast_context.hpp"
#include "ast_declaration.hpp"
#include "ast_direct_declarator.hpp"
#include "ast_function_call.hpp"
#include "ast_function_definition.hpp"
#include "ast_identifier.hpp"
#include "ast_iteration.hpp"
#include "ast_jump_statement.hpp"
#include "ast_logical.hpp"
#include "ast_node.hpp"
#include "ast_parameter.hpp"
#include "ast_pointer.hpp"
#include "ast_pointer_declarator.hpp"
#include "ast_postfix_inc.hpp"
#include "ast_selection.hpp"
#include "ast_sizeof_string.hpp"
#include "ast_struct.hpp"
#include "ast_struct_access.hpp"
#include "ast_type_specifier.hpp"

ast::NodePtr ParseAST(std::string file_name);
