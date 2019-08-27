%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ast.h"
#include "eval.h"
#include "debug.h"

FILE *yyin;
int yyerror(char*);
int yylex();

BLOCK first_block;
BLOCK current_block;

int yydebug = 1;
%}


%start program
 //%debug
 %type <ast> exp
%type <string> sym

%token <number> NUMBER
%token <string> PLUS
%token <string> LPAREN RPAREN
%token <string> STRING

%union {
  struct ast * ast;
  int number;
  char * sym;
  char * string;
}

%%

program:
| program exp {

  current_block = add_block(current_block, $2);
  if (first_block == NULL) { first_block = current_block; }
 };

exp:
  NUMBER { $$ = make_number_node($1); }
| LPAREN sym exp exp RPAREN { $$ = make_ast_node($2, $3, $4); }
;

sym: PLUS;

%%

int main(int argc, char **argv) {
  yyin = fopen(argv[1], "r");

  first_block = NULL;
  current_block = NULL;

  yyparse();
  fclose(yyin);

  current_block = first_block;

  while(current_block != NULL && current_block->root_node != NULL) {
    print_ast(current_block->root_node, 0);

    /*
    NODE result = eval(current_block->root_node);
    if (result != NULL && result->node_type == NODE_NUM) {
      printf("result is %d \n", result->u.number);
    }
    */

    current_block = current_block->next;
  }

  return 0;
}

int yyerror(char * message) {
    printf("yyerror %s\n", message);

    return 0;
}
