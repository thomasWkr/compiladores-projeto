%{
#include <stdio.h>

int yylex(void);
void yyerror (char const *mensagem);
int get_line_number(void);
%}

%token TK_PR_AS
%token TK_PR_DECLARE
%token TK_PR_ELSE
%token TK_PR_FLOAT
%token TK_PR_IF
%token TK_PR_INT
%token TK_PR_IS
%token TK_PR_RETURN
%token TK_PR_RETURNS
%token TK_PR_WHILE
%token TK_PR_WITH
%token TK_OC_LE
%token TK_OC_GE
%token TK_OC_EQ
%token TK_OC_NE
%token TK_ID
%token TK_LI_INT
%token TK_LI_FLOAT
%token TK_ER

%define parse.error verbose

%%
program: list ';';
program: ;

type: TK_PR_INT | TK_PR_FLOAT;

literal: TK_LI_INT | TK_LI_FLOAT;

element: def_func | decl_var;

list: element | element ',' list;

parameter: TK_ID TK_PR_AS type;
parameter_list: parameter | parameter ',' parameter_list;

header: TK_ID TK_PR_RETURNS type TK_PR_IS;
header: TK_ID TK_PR_RETURNS type TK_PR_WITH parameter_list TK_PR_IS;

simple_command: command_block | var | assign | func_call | return_call | flux_controll;

command_seq: ;
command_seq: simple_command command_seq;

command_block: '[' command_seq ']';

def_func: header command_block;
decl_var: TK_PR_DECLARE TK_ID TK_PR_AS type;

var: decl_var TK_PR_WITH literal | decl_var;

assign: TK_ID TK_PR_IS expr;

func_call: TK_ID '(' args_list ')'; | TK_ID '(' ')';

args_list: expr ',' args_list | expr;

return_call: TK_PR_RETURN expr TK_PR_AS type;

flux_controll: cond_block | iter_block;

cond_block: TK_PR_IF '(' expr ')' command_block;
cond_block: TK_PR_IF '(' expr ')' command_block TK_PR_ELSE command_block;

iter_block: TK_PR_WHILE '(' expr ')' command_block;

n0: TK_ID | literal | func_call | '(' expr ')';
n1: '+' n1 | '-' n1 | '!' n1 | n0;
n2: n1 '*' n2 | n1 '/' n2 | n1 '%' n2 | n1;
n3: n2 '+' n3 | n2 '-' n3 | n2;
n4: n3 '<' n4 | n3 '>' n4  | n3 TK_OC_LE n4 | n3 TK_OC_GE n4 | n3;
n5: n4 TK_OC_EQ n5 | n4 TK_OC_NE n5 | n4;
n6: n5 '&' n6 | n5;
n7: n6 '|' n7 | n6;
expr: n7;

%%

void yyerror(char const *mensagem){
    printf("Erro no linha: %d , %s\n", get_line_number(), mensagem);
}