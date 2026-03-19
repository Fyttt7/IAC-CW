// Adapted from: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html

// TODO: you can either continue adding to this grammar file or
// rename parser_full.y to parser.y once you're happy with
// how this example works.

%code requires {
	#include "ast.hpp"
	using namespace ast;

	extern int yylineno;
	extern char* yytext;
	extern Node* g_root;
	extern FILE* yyin;

	int yylex(void);
	void yyerror(const char*);
	int yylex_destroy(void);
}

%define parse.error detailed
%define parse.lac full

%union {
  Node*				    node;
  NodeList*			  node_list;
  int          		number_int;
  double       		number_float;
  std::string*		string;
  TypeSpecifier 	type_specifier;
  yytokentype  		token;
}

%token IDENTIFIER INT_CONSTANT FLOAT_CONSTANT STRING_LITERAL
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP AND_OP OR_OP
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%token TYPE_NAME TYPEDEF EXTERN STATIC AUTO REGISTER SIZEOF
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS
%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token UNKNOWN

/* node list */
%type <node_list> translation_unit
%type <node_list> statement_list declaration_list init_declarator_list parameter_list argument_expression_list struct_member_list

/* ast node*/
/* dec */
%type <node> external_declaration function_definition declaration init_declarator
%type <node> declarator direct_declarator
%type <node> parameter_declaration

/* statement */
%type <node> statement compound_statement jump_statement selection_statement iteration_statement expression_statement

/* expression pri low->high */
%type <node> expression assignment_expression conditional_expression
%type <node> logical_or_expression logical_and_expression
%type <node> inclusive_or_expression exclusive_or_expression and_expression
%type <node> equality_expression relational_expression
%type <node> shift_expression additive_expression multiplicative_expression
%type <node> cast_expression unary_expression postfix_expression primary_expression

/* %type <node> declaration init_declarator
%type <node> external_declaration function_definition primary_expression postfix_expression
%type <node> unary_expression cast_expression multiplicative_expression additive_expression shift_expression relational_expression
%type <node> equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression
%type <node> conditional_expression assignment_expression expression declarator direct_declarator statement compound_statement jump_statement

%type <node_list> translation_unit declaration_list init_declarator_list statement_list */

%type <string> IDENTIFIER STRING_LITERAL
%type <number_float> FLOAT_CONSTANT
%type <type_specifier> type_specifier declaration_specifiers
%type <number_int> INT_CONSTANT


%start ROOT
%%

ROOT
	: translation_unit { g_root = $1; }

translation_unit
    : external_declaration { $$ = new NodeList(NodePtr($1)); } // smart ptr?
    | translation_unit external_declaration { $1->PushBack(NodePtr($2)); $$=$1; }
    ;

external_declaration
  : function_definition { $$ = $1; }
  | declaration { $$ = $1; }
  | STRUCT IDENTIFIER '{' struct_member_list '}' ';' {
      $$ = new StructDefinition(*$2, NodePtr($4));
      delete $2;
  }
  ;

function_definition
	: declaration_specifiers declarator compound_statement {
		$$ = new FunctionDefinition($1, NodePtr($2), NodePtr($3));
	}
	;

declaration
  : declaration_specifiers ';' { $$ = new Declaration($1, nullptr); }
  | declaration_specifiers init_declarator_list ';' { $$ = new Declaration($1, NodePtr($2)); }
  | STRUCT IDENTIFIER IDENTIFIER ';' { $$ = new StructVarDeclaration(*$2, *$3); delete $2; delete $3; }
  ;

init_declarator_list
    : init_declarator { $$ = new NodeList(NodePtr($1)); }
    | init_declarator_list ',' init_declarator { $1->PushBack(NodePtr($3)); $$=$1; }
    ;

init_declarator
    : declarator {
        $$ = new InitDeclarator(NodePtr($1), nullptr);
    }
    | declarator '=' assignment_expression {
        $$ = new InitDeclarator(NodePtr($1), NodePtr($3));
    }
    ;

declaration_specifiers
	: type_specifier { $$ = $1; }
	;

type_specifier
  : INT { $$ = TypeSpecifier::INT; }
  | CHAR { $$ = TypeSpecifier::CHAR; }
  | VOID { $$ = TypeSpecifier::VOID; }
  | STRUCT { $$ = TypeSpecifier::STRUCT; }
  ;

declarator
  : direct_declarator { $$ = $1; }
  | '*' declarator { $$ = new PointerDeclarator(NodePtr($2)); }
  ;

direct_declarator
  : IDENTIFIER {
    $$ = new Identifier(std::move(*$1));
    delete $1;
  }
  | direct_declarator '(' ')' {
    $$ = new DirectDeclarator(NodePtr($1), nullptr);
  }
  | direct_declarator '(' VOID ')' {
    $$ = new DirectDeclarator(NodePtr($1), nullptr);
  }
  | direct_declarator '(' parameter_list ')' {
    $$ = new DirectDeclarator(NodePtr($1), NodePtr($3));
  }
  | direct_declarator '[' INT_CONSTANT ']' {
    $$ = new DirectDeclarator(NodePtr($1), $3);
  }
  ;

parameter_list
  : parameter_declaration { $$ = new NodeList(NodePtr($1)); }
  | parameter_list ',' parameter_declaration { $1->PushBack(NodePtr($3)); $$=$1; }
  ;

parameter_declaration
  : declaration_specifiers declarator {
    $$ = new Parameter($1, NodePtr($2));
  }
  ;






statement
	: jump_statement { $$ = $1; }
  | expression_statement { $$ = $1; }
  | selection_statement { $$ = $1; }
	| iteration_statement { $$ = $1; }
	| compound_statement  { $$ = $1; }  // {} inside if
	;

compound_statement
    : '{' '}' { $$ = new CompoundStatement(nullptr, nullptr); }
    | '{' statement_list '}' { $$ = new CompoundStatement(nullptr, NodePtr($2)); }
    | '{' declaration_list '}' { $$ = new CompoundStatement(NodePtr($2), nullptr); }
    | '{' declaration_list statement_list '}' {
        $$ = new CompoundStatement(NodePtr($2), NodePtr($3));
    }
    ;

declaration_list
    : declaration { $$ = new NodeList(NodePtr($1)); }
    | declaration_list declaration { $1->PushBack(NodePtr($2)); $$=$1; }
    ;

statement_list
	: statement { $$ = new NodeList(NodePtr($1)); }
	| statement_list statement { $1->PushBack(NodePtr($2)); $$=$1; }
	;

expression_statement
  : ';' { $$ = nullptr; }
	| expression ';' { $$ = $1; }
	;

selection_statement
  : IF '(' expression ')' statement {
      $$ = new IfStatement(NodePtr($3), NodePtr($5), nullptr);
  }
  | IF '(' expression ')' statement ELSE statement {
      $$ = new IfStatement(NodePtr($3), NodePtr($5), NodePtr($7));
  }
  ;

iteration_statement
  : WHILE '(' expression ')' statement {
      $$ = new WhileStatement(NodePtr($3), NodePtr($5));
  }
  | FOR '(' expression_statement expression_statement ')' statement {
      $$ = new ForStatement(NodePtr($3), NodePtr($4), nullptr, NodePtr($6));
  }
  | FOR '(' expression_statement expression_statement expression ')' statement {
      $$ = new ForStatement(NodePtr($3), NodePtr($4), NodePtr($5), NodePtr($7));
  }
  ;

jump_statement
	: RETURN ';' {
		$$ = new ReturnStatement(nullptr);
	}
	| RETURN expression ';' {
		$$ = new ReturnStatement(NodePtr($2));
	}
  ;

argument_expression_list
  : assignment_expression { $$ = new NodeList(NodePtr($1)); }
  | argument_expression_list ',' assignment_expression { $1->PushBack(NodePtr($3)); $$=$1; }
  ;

primary_expression
    : IDENTIFIER {
        $$ = new Identifier(std::move(*$1));
        delete $1;
    }
    | INT_CONSTANT { $$ = new IntConstant($1); }
    | '(' expression ')' { $$ = $2; }
    | STRING_LITERAL { $$ = new StringLiteral(std::move(*$1)); delete $1; }
    ;

postfix_expression
    : primary_expression
      { $$ = $1; }
    | postfix_expression '(' ')'
      { $$ = new FunctionCall(NodePtr($1), nullptr); }
    | postfix_expression '(' argument_expression_list ')'
      { $$ = new FunctionCall(NodePtr($1), NodePtr($3)); }
    | postfix_expression '[' expression ']'
      {
        NodePtr addr = NodePtr(new AddExpression(NodePtr($1), NodePtr($3)));
        $$ = new DereferenceExpression(std::move(addr));
      }
    | postfix_expression INC_OP
      { $$ = new PostfixInc(NodePtr($1)); }
    | postfix_expression '.' IDENTIFIER {
      $$ = new StructMemberAccess(NodePtr($1), *$3);
      delete $3;
    }
    ;

unary_expression
    : postfix_expression
      { $$ = $1; }
    | '-' unary_expression
      { $$ = new NegExpression(NodePtr($2)); }
    | '!' unary_expression
      { $$ = new LNotExpression(NodePtr($2)); }
    | '&' cast_expression
      { $$ = new AddressOfExpression(NodePtr($2)); }
    | '*' cast_expression
      { $$ = new DereferenceExpression(NodePtr($2)); }
    | SIZEOF unary_expression { $$ = new SizeOfExpression(NodePtr($2)); }
    ;

cast_expression
    : unary_expression
      { $$ = $1; }
    ;

multiplicative_expression
    : cast_expression
      { $$ = $1; }
    | multiplicative_expression '*' cast_expression
      { $$ = new MulExpression(NodePtr($1), NodePtr($3)); }
    | multiplicative_expression '/' cast_expression
      { $$ = new DivExpression(NodePtr($1), NodePtr($3)); }
    | multiplicative_expression '%' cast_expression
      { $$ = new ModExpression(NodePtr($1), NodePtr($3)); }
    ;

additive_expression
    : multiplicative_expression
    {
        $$ = $1;
    }
    | additive_expression '+' multiplicative_expression
    {
        $$ = new AddExpression(NodePtr($1), NodePtr($3));
    }
    | additive_expression '-' multiplicative_expression
    {
        $$ = new SubExpression(NodePtr($1), NodePtr($3));
    }
    ;

shift_expression
	: additive_expression { $$ = $1; }
	| shift_expression LEFT_OP additive_expression
	  { $$ = new ShiftLeftExpression(NodePtr($1), NodePtr($3)); }
	| shift_expression RIGHT_OP additive_expression
	  { $$ = new ShiftRightExpression(NodePtr($1), NodePtr($3)); }
	;

relational_expression
	: shift_expression { $$ = $1; }
	| relational_expression '<' shift_expression
	  { $$ = new LessThanExpression(NodePtr($1), NodePtr($3)); }
	| relational_expression '>' shift_expression
	  { $$ = new GreaterThanExpression(NodePtr($1), NodePtr($3)); }
	| relational_expression LE_OP shift_expression // <=
	  { $$ = new LessEqualExpression(NodePtr($1), NodePtr($3)); }
	| relational_expression GE_OP shift_expression // >=
	  { $$ = new GreaterEqualExpression(NodePtr($1), NodePtr($3)); }
	;

equality_expression
	: relational_expression { $$ = $1; }
	| equality_expression EQ_OP relational_expression
	  { $$ = new EqualExpression(NodePtr($1), NodePtr($3)); }
	| equality_expression NE_OP relational_expression // !=
	  { $$ = new NotEqualExpression(NodePtr($1), NodePtr($3)); }
	;

and_expression
	: equality_expression { $$ = $1; }
	| and_expression '&' equality_expression
	  { $$ = new BitAndExpression(NodePtr($1), NodePtr($3)); }
	;

exclusive_or_expression
	: and_expression { $$ = $1; }
	| exclusive_or_expression '^' and_expression
	  { $$ = new BitXorExpression(NodePtr($1), NodePtr($3)); }
	;

inclusive_or_expression
	: exclusive_or_expression { $$ = $1; }
	| inclusive_or_expression '|' exclusive_or_expression
	  { $$ = new BitOrExpression(NodePtr($1), NodePtr($3)); }
	;

logical_and_expression
	: inclusive_or_expression { $$ = $1; }
	| logical_and_expression AND_OP inclusive_or_expression
	  { $$ = new LAndExpression(NodePtr($1), NodePtr($3)); }
	;

logical_or_expression
	: logical_and_expression { $$ = $1; }
	| logical_or_expression OR_OP logical_and_expression
	  { $$ = new LOrExpression(NodePtr($1), NodePtr($3)); }
	;

conditional_expression
	: logical_or_expression { $$ = $1; }
	;

assignment_expression
    : unary_expression '=' assignment_expression {
        $$ = new AssignmentExpression(NodePtr($1), NodePtr($3));
    }
    | conditional_expression { $$ = $1; }
    ;

expression
	: assignment_expression
	;

struct_member_list
  : type_specifier IDENTIFIER ';' {
      $$ = new NodeList(NodePtr(new StructMemberDecl($1, *$2)));
      delete $2;
  }
  | struct_member_list type_specifier IDENTIFIER ';' {
      $1->PushBack(NodePtr(new StructMemberDecl($2, *$3)));
      delete $3;
      $$ = $1;
  }
  ;


%%

void yyerror (const char *s)
{
  std::cerr << "Error: " << s << " at line " << yylineno;
  std::cerr << " near '" << yytext << "'" << std::endl;
  std::exit(1);
}

Node* g_root;

NodePtr ParseAST(std::string file_name)
{
  yyin = fopen(file_name.c_str(), "r");
  if (yyin == nullptr) {
    std::cerr << "Couldn't open input file: " << file_name << std::endl;
    std::exit(1);
  }

  g_root = nullptr;
  yyparse();

  fclose(yyin);
  yylex_destroy();

  return NodePtr(g_root);
}
