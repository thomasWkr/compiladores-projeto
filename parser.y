%{
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "asd.h"

int yylex(void);
void yyerror (char const *mensagem);
int get_line_number(void);
extern asd_tree_t *arvore;

%}

%union{
	asd_tree_t *no;
	value_t *valor_lexico;
}

%destructor{
	if ($$ != NULL && $$ != arvore){
		asd_free($$);
	}
} <no>;

%token<valor_lexico> TK_PR_AS
%token<valor_lexico> TK_PR_DECLARE
%token<valor_lexico> TK_PR_ELSE
%token<valor_lexico> TK_PR_FLOAT
%token<valor_lexico> TK_PR_IF
%token<valor_lexico> TK_PR_INT
%token<valor_lexico> TK_PR_IS
%token<valor_lexico> TK_PR_RETURN
%token<valor_lexico> TK_PR_RETURNS
%token<valor_lexico> TK_PR_WHILE
%token<valor_lexico> TK_PR_WITH
%token<valor_lexico> TK_OC_LE
%token<valor_lexico> TK_OC_GE
%token<valor_lexico> TK_OC_EQ
%token<valor_lexico> TK_OC_NE
%token<valor_lexico> TK_ID
%token<valor_lexico> TK_LI_INT
%token<valor_lexico> TK_LI_FLOAT
%token<valor_lexico> TK_ER

%type<no> program
%type<no> list
%type<no> n0
%type<no> n1
%type<no> n2
%type<no> n3
%type<no> n4
%type<no> n5
%type<no> n6
%type<no> n7
%type<no> type
%type<no> literal
%type<no> element
%type<no> parameter
%type<no> parameter_list
%type<no> header
%type<no> simple_command
%type<no> command_seq
%type<no> command_block
%type<no> command_block_func
%type<no> def_func
%type<no> decl_var
%type<no> var
%type<no> assign
%type<no> func_call
%type<no> args_list
%type<no> return_call
%type<no> flux_controll
%type<no> cond_block
%type<no> iter_block
%type<no> expr

%define parse.error verbose

%%

// ============================= GENERAL ==============================

program: {create_scope();} list {destroy_scope();} ';' {arvore = $2;};
program: { arvore = NULL; };

// Adds type declaration to the tree
type: TK_PR_INT {$$ = asd_new("int", NULL, INT);};
type: TK_PR_FLOAT {$$ = asd_new("float", NULL, FLOAT);};

literal: TK_LI_INT {
	char *key = strdup($1->lexeme);
	content_t *content= create_content(INT, LITERAL, NULL, create_lexic_value($1->nature, key, $1->line_number));
	update_table(content, key);
	$$ = asd_new($1->lexeme, $1, INT); 
};
literal: TK_LI_FLOAT { 
	char *key = strdup($1->lexeme);
	content_t *content= create_content(FLOAT, LITERAL, NULL, create_lexic_value($1->nature, key, $1->line_number));
	update_table(content, key);
	$$ = asd_new($1->lexeme, $1, FLOAT); 
};

element: def_func { $$ = $1; };
element: decl_var { $$ = NULL; asd_free($1);}; // Not initialized Vars dont exist on the tree

list: element { $$ = $1; }; 
list: element ',' list { 
	if ($1 == NULL) { // If element is NULL, uses next as head
		$$ = $3;
	}
	else if ($3 != NULL){ 
		asd_add_child($1, $3);
	} else { // If list is NULL, doesnt add child
		$$ = $1; 
	}
}

// ============================= COMMANDS =============================

simple_command: command_block { $$ = $1; }; 
simple_command: var { $$ = $1; }; 
simple_command: assign { $$ = $1; };
simple_command: func_call { $$ = $1; };
simple_command: return_call { $$ = $1; };
simple_command: flux_controll { $$ = $1; };

command_seq: simple_command { $$ = $1;};
command_seq: simple_command command_seq { 
	if ($1 == NULL) { // If command is NULL, jump to the next
		$$ = $2;
	}
	else if ($2 != NULL){ 
		asd_add_child($1, $2); 
	} else {
		$$ = $1; 
	}
};

command_block: '[' {create_scope();} command_seq {destroy_scope();} ']' { $$ = $3; }; 
command_block: '[' ']' { $$ = NULL; };

command_block_func: '[' command_seq ']' { $$ = $2; }; // Function command block doesnt create scope
command_block_func: '[' ']' { $$ = NULL; };

// ============================ FUNCTIONS =============================

parameter: TK_ID TK_PR_AS type { 
	char *key = strdup($1->lexeme);
	content_t *content_p= create_content($3->type, ID, NULL, $1); 
	update_table(content_p, key); // Adds parameter to Symbol Table
	get_latest_function()->content->args = add_arg(
		get_latest_function()->content->args, $3->type); // Adds parameter to latest function list 

	$$ = NULL;
	asd_free($3);
};
parameter_list: parameter { $$ = $1; }; 
parameter_list: parameter ',' parameter_list { $$ = $1; };
 
header: TK_ID TK_PR_RETURNS type {
	char *key = strdup($1->lexeme);
	content_t *content_f= create_content(
		$3->type, FUNCTION, NULL, create_lexic_value($1->nature, key, $1->line_number));
	update_table(content_f, key); // Adds function to symbol table
	create_scope();} 
	TK_PR_IS 
	{
	$$ = asd_new( $1->lexeme, $1, $3->type); 
	asd_free($3);
}; 
header: TK_ID TK_PR_RETURNS type TK_PR_WITH {
	char *key = strdup($1->lexeme);
	content_t *content_f= create_content($3->type, FUNCTION, NULL, create_lexic_value($1->nature, key, $1->line_number));
	update_table(content_f, key); // Adds function to symbol table
	create_scope();} 
	parameter_list TK_PR_IS 
	{ 
	$$ = asd_new( $1->lexeme, $1, $3->type); 
	asd_free($3);
};

def_func: header command_block_func {destroy_scope();} { 
	$$ = $1;
	if ($2 != NULL) 
		asd_add_child($1, $2); 
};

return_call: TK_PR_RETURN expr TK_PR_AS type { 
	compare_type($2->type, $4->type, get_line_number()); // Compares expression type to returned type
	symbol_t *latest_function = get_latest_function();
	compare_type($2->type, latest_function->content->type, get_line_number()); // Compares return type to function type

	$$ = asd_new("return", NULL, $4->type); 
	asd_add_child($$, $2); 
	asd_free($4);
};

// ============================ VARIABLES =============================

decl_var: TK_PR_DECLARE TK_ID TK_PR_AS type {
	char *key = strdup($2->lexeme);
	content_t *content= create_content(
		$4->type, ID, NULL, create_lexic_value($2->nature, key, $2->line_number));
	update_table(content, key); // Adds var to symbol table
	$$ = asd_new($2->lexeme, $2, $4->type);
	asd_free($4);
};

var: decl_var TK_PR_WITH literal { 
	compare_type($1->type, $3->type, get_line_number()); // Compares literal type to var type
	$$ = asd_new("with", NULL, $1->type); 
	asd_add_child($$,$3); 
	asd_add_child($$,$1); 
};
var: decl_var { $$ = NULL; asd_free($1);};

assign: TK_ID TK_PR_IS expr { 
	check_declared($1, $1->lexeme); 
	type_t type_id = get_symbol_from_stack($1->lexeme)->content->type;
	compare_type(type_id, $3->type, $1->line_number); // Compares var type to literal type

	$$ = asd_new("is", NULL, type_id);
	asd_tree_t *id = asd_new($1->lexeme, $1, type_id); 
	asd_add_child($$, id); 
	asd_add_child($$, $3);
};

// ========================== FUNCTION CALLS ==========================

args_list: expr ',' args_list { 
	update_args_list($1->type); // Adds argument to temporary arguments list
	$$ = $1; 
	asd_add_child($$, $3); 
}; 
args_list: expr { 
	update_args_list($1->type); // Adds argument to temporary arguments list
	$$ = $1; 
};
func_call: TK_ID '(' {
	create_args_list(); // Creates arguments list on the top of the stack
	$1->nature = FUNCTION;
	check_declared($1, $1->lexeme);} 
	args_list ')' 
	{
	symbol_t *function = get_symbol_from_stack($1->lexeme);
	compare_args(function->content->args, $1); // Compares function parameters to function call arguments

	destroy_args_list();

	char name[256];
	sprintf(name, "call %s", $1->lexeme);
	$$ = asd_new(name, $1, function->content->type);
	asd_add_child($$, $4); 
};

func_call: TK_ID '(' ')' {
	$1->nature = FUNCTION;
	check_declared($1, $1->lexeme);
	symbol_t *function = get_symbol_from_stack($1->lexeme);

	create_args_list(); // Creates empty temporary arguments list
	compare_args(function->content->args, $1); // Compares empty lists
	destroy_args_list();

	char name[256];
	sprintf(name, "call %s", $1->lexeme);
	$$ = asd_new(name, $1, function->content->type);
};

// ========================= FLUX CONTROLLERS =========================

flux_controll: cond_block { $$ = $1; };
flux_controll: iter_block { $$ = $1; };

cond_block: TK_PR_IF '(' expr ')' command_block { 
	$$ = asd_new("if", NULL, $3->type); 
	asd_add_child($$, $3); 
	if ($5 != NULL) 
		asd_add_child($$, $5); 
};
cond_block: TK_PR_IF '(' expr ')' command_block TK_PR_ELSE command_block {
	$$ = asd_new("if", NULL, $3->type); 
	asd_add_child($$, $3);

	if ($5 && $7){ // If both blocks aren't NULL, compares their types and adds to tree
		compare_type($5->type, $7->type, get_line_number());
		asd_add_child($$, $5); 
		asd_add_child($$, $7);  
	} else if ($5 && !$7){ // If 'else' block is NULL -> Type Error
        printf("Row: %d, else block is empty\n", get_line_number());
		asd_free($5);
		asd_free($3);
        destroy_scope();
        exit(ERR_WRONG_TYPE);
	} else if (!$5 && $7){ // If 'if' block is NULL -> Type Error
        printf("Row: %d, if block is empty\n", get_line_number());
		asd_free($7);
		asd_free($3);
        destroy_scope();
        exit(ERR_WRONG_TYPE);
	} 
};

iter_block: TK_PR_WHILE '(' expr ')' command_block { 
	$$ = asd_new("while", NULL, $3->type); 
	asd_add_child($$, $3); 
	if ($5 != NULL) 
		asd_add_child($$, $5); 
};

// =========================== EXPRESSIONS ============================

// TO-DO
// - generalizar o codigo das expressões
n0: TK_ID { 
	check_declared($1, $1->lexeme);
	type_t type_id = get_symbol_from_stack($1->lexeme)->content->type;

	$$ = asd_new($1->lexeme, $1, type_id); 
}; 
n0: literal { $$ = $1; };
n0: func_call { $$ = $1; };
n0: '(' expr ')' { $$ = $2; };

n1: '+' n1 { 
	$$ = asd_new("+", NULL, $2->type); 
	asd_add_child($$, $2); 
};
n1: '-' n1 { 
	$$ = asd_new("-", NULL, $2->type); 
	asd_add_child($$, $2); 
};
n1: '!' n1 { 
	$$ = asd_new("!", NULL, $2->type); 
	asd_add_child($$, $2); 
};
n1: n0 { $$ = $1; };

n2: n2 '*' n1 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("*", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); };
n2: n2 '/' n1 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("/", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); };
n2: n2 '%' n1 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("\%", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); }; 
n2: n1 { $$ = $1; };

n3: n3 '+' n2 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("+", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3);
};
n3: n3 '-' n2 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("-", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); 
};
n3: n2 { $$ = $1; };

n4: n4 '<' n3 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("<", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); 
};
n4: n4 '>' n3 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new(">", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); 
};
n4: n4 TK_OC_LE n3 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("<=", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); 
};
n4: n4 TK_OC_GE n3 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new(">=", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); 
};
n4: n3 { $$ = $1; };

n5: n5 TK_OC_EQ n4 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("==", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); 
};
n5: n5 TK_OC_NE n4 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("!=", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); 
}; 
n5: n4 { $$ = $1; };

n6: n6 '&' n5 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("&", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); 
};
n6: n5 { $$ = $1; };

n7: n7 '|' n6 { 
	compare_type($1->type, $3->type, get_line_number());

	$$ = asd_new("|", NULL, $1->type); 
	asd_add_child($$, $1); 
	asd_add_child($$, $3); 
};
n7: n6 { $$ = $1; };

expr: n7 { $$ = $1; };

%%

void yyerror(char const *mensagem){
    printf("Row: %d, %s\n", get_line_number(), mensagem);
}
