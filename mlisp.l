%{
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}

%%

[0-9]+ { yylval.number = atoi(yytext); return NUMBER; }
"+" { yylval.string = "+"; return PLUS; };
"(" return LPAREN;
")" return RPAREN;

\;(.*) {};

\"(.)*\" return STRING;

[ \t\n] ;

%%

int yywrap(void) { return 1; }
