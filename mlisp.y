%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

FILE *yyin;
int yyerror(char*);
int yylex();
%}

%start program

%token NUMBER
%token PLUS
%token POWER
%token MINUS
%token DIVIDE
%token LPAREN
%token RPAREN
%token STRING

%%

program:
 | program exp { printf("%d\n", $2); }

exp:
 NUMBER
 | LPAREN PLUS exp exp RPAREN { $$ = $3 + $4; }
 | LPAREN MINUS exp exp RPAREN { $$ = $3 - $4; }
 | LPAREN POWER exp exp RPAREN { $$ = $3 * $4; }
 | LPAREN DIVIDE exp exp RPAREN { $$ = $3 / $4; }

;

%%

void main(int argc, char **argv) {
  yyin = fopen(argv[1], "r");
  yyparse();
  fclose(yyin);
}

int yyerror(char * message) {
    printf("yyerror %s", message);
}
