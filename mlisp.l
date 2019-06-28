%{
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>

%}

%%

[0-9]+ { yylval = atoi(yytext); return NUMBER; }
"+" return PLUS;
"-" return MINUS;
"*" return POWER;
"(" return LPAREN;
")" return RPAREN;

\;(.*) {};

\"(.)*\" return STRING;

[ \t\n] ;

%%

int yywrap(void) { return 1; }
