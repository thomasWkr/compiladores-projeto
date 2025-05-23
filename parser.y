%{
#include <stdio.h>
#include "asd.h"
#include "table.h"

int yylex(void);
void yyerror (char const *mensagem);
int get_line_number(void);
extern asd_tree_t *arvore;

%}

%union{
	asd_tree_t *no;
	valor_t *valor_lexico;
}

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
program: create_scope list destroy_scope ';' {arvore = $1;};
program: { arvore = NULL; };

type: TK_PR_INT {$$ = NULL;};
type: TK_PR_FLOAT {$$ = NULL;};

literal: TK_LI_INT { 
	$$ = asd_new($1->lexema, $1); 
};
literal: TK_LI_FLOAT { 
	$$ = asd_new($1->lexema, $1); 
};

element: def_func { $$ = $1; };
element: decl_var { $$ = NULL; asd_free($1);};

list: element { $$ = $1; }; 
list: element ',' list { 
	if ($1 == NULL) {
		$$ = $3;
	}
	else if ($3 != NULL){ 
		asd_add_child($1, $3);
	} else {
		$$ = $1; 
	}
}

parameter: TK_ID TK_PR_AS type { 
	$$ = NULL;
	free($1->lexema); 
	free($1);
};
parameter_list: parameter { $$ = $1; }; 
parameter_list: parameter ',' parameter_list { $$ = $1; };

header: TK_ID TK_PR_RETURNS type TK_PR_IS { 
	$$ = asd_new( $1->lexema, $1); 
}; 
header: TK_ID TK_PR_RETURNS type TK_PR_WITH create_scope parameter_list TK_PR_IS { 
	$$ = asd_new( $1->lexema, $1 ); 
	free($5);
};

simple_command: command_block { $$ = $1; }; 
simple_command: var { $$ = $1; }; 
simple_command: assign { $$ = $1; };
simple_command: func_call { $$ = $1; };
simple_command: return_call { $$ = $1; };
simple_command: flux_controll { $$ = $1; };

command_seq: simple_command { $$ = $1;};
command_seq: simple_command command_seq { 
	if ($1 == NULL) {
		$$ = $2;
	}
	else if ($2 != NULL){ 
		asd_add_child($1, $2); 
	} else {
		$$ = $1; 
	}
};

command_block: '[' create_scope command_seq destroy_scope ']' { $$ = $2; }; 
command_block: '[' ']' { $$ = NULL; }; //problema

def_func: header command_block destroy_scope { 
	$$ = $1;
	if ($2 != NULL) 
		asd_add_child($1, $2); 
};
decl_var: TK_PR_DECLARE TK_ID TK_PR_AS type { 
	$$ = asd_new($2->lexema, $2); 
};

var: decl_var TK_PR_WITH literal { $$ = asd_new("with", NULL); asd_add_child($$,$3); asd_add_child($$,$1); };
var: decl_var { $$ = NULL; asd_free($1);};

assign: TK_ID TK_PR_IS expr { 
	$$ = asd_new("is", NULL);
	asd_tree_t *id = asd_new($1->lexema, $1); 
	asd_add_child($$, id); 
	asd_add_child($$, $3);
};

func_call: TK_ID '(' args_list ')' {
	char name[256];
	sprintf(name, "call %s", $1->lexema);
	$$ = asd_new(name, $1);
	asd_add_child($$, $3); 
};

func_call: TK_ID '(' ')' {
	char name[256];
	sprintf(name, "call %s", $1->lexema);
	$$ = asd_new(name, $1);
};

args_list: expr ',' args_list { $$ = $1; asd_add_child($$, $3); }; 
args_list: expr { $$ = $1; };

return_call: TK_PR_RETURN expr TK_PR_AS type { $$ = asd_new("return", NULL); asd_add_child($$, $2); };

flux_controll: cond_block { $$ = $1; };
flux_controll: iter_block { $$ = $1; };

cond_block: TK_PR_IF '(' expr ')' command_block { 
	$$ = asd_new("if", NULL); 
	asd_add_child($$, $3); 
	if ($5 != NULL) 
		asd_add_child($$, $5); 
};
cond_block: TK_PR_IF '(' expr ')' command_block TK_PR_ELSE command_block { 
	$$ = asd_new("if", NULL); 
	asd_add_child($$, $3);
	if ($5 != NULL) 
		asd_add_child($$, $5); 
	if ($7 != NULL) 
		asd_add_child($$, $7);  
};

iter_block: TK_PR_WHILE '(' expr ')' command_block { 
	$$ = asd_new("while", NULL); 
	asd_add_child($$, $3); 
	if ($5 != NULL) 
		asd_add_child($$, $5); 
};

n0: TK_ID { 
	$$ = asd_new($1->lexema, $1); 
}; 
n0: literal { $$ = $1; };
n0: func_call { $$ = $1; };
n0: '(' expr ')' { $$ = $2; };

n1: '+' n1 { $$ = asd_new("+", NULL); asd_add_child($$, $2); };
n1: '-' n1 { $$ = asd_new("-", NULL); asd_add_child($$, $2); };
n1: '!' n1 { $$ = asd_new("!", NULL); asd_add_child($$, $2); };
n1: n0 { $$ = $1; };

n2: n2 '*' n1 { $$ = asd_new("*", NULL); asd_add_child($$, $1); asd_add_child($$, $3); };
n2: n2 '/' n1 { $$ = asd_new("/", NULL); asd_add_child($$, $1); asd_add_child($$, $3); };
n2: n2 '%' n1 { $$ = asd_new("\%", NULL); asd_add_child($$, $1); asd_add_child($$, $3); }; 
n2: n1 { $$ = $1; };

n3: n3 '+' n2 { $$ = asd_new("+", NULL); asd_add_child($$, $1); asd_add_child($$, $3); };
n3: n3 '-' n2 { $$ = asd_new("-", NULL); asd_add_child($$, $1); asd_add_child($$, $3); };
n3: n2 { $$ = $1; };

n4: n4 '<' n3 { $$ = asd_new("<", NULL); asd_add_child($$, $1); asd_add_child($$, $3); };
n4: n4 '>' n3 { $$ = asd_new(">", NULL); asd_add_child($$, $1); asd_add_child($$, $3); };
n4: n4 TK_OC_LE n3 { $$ = asd_new("<=", NULL); asd_add_child($$, $1); asd_add_child($$, $3); };
n4: n4 TK_OC_GE n3 { $$ = asd_new(">=", NULL); asd_add_child($$, $1); asd_add_child($$, $3); };
n4: n3 { $$ = $1; };

n5: n5 TK_OC_EQ n4 { $$ = asd_new("==", NULL); asd_add_child($$, $1); asd_add_child($$, $3); };
n5: n5 TK_OC_NE n4 { $$ = asd_new("!=", NULL); asd_add_child($$, $1); asd_add_child($$, $3); }; 
n5: n4 { $$ = $1; };

n6: n6 '&' n5 { $$ = asd_new("&", NULL); asd_add_child($$, $1); asd_add_child($$, $3); };
n6: n5 { $$ = $1; };

n7: n7 '|' n6 { $$ = asd_new("|", NULL);  asd_add_child($$, $1); asd_add_child($$, $3); };
n7: n6 { $$ = $1; };

expr: n7 { $$ = $1; };

%%

void yyerror(char const *mensagem){
    printf("Row: %d, %s\n", get_line_number(), mensagem);
}
