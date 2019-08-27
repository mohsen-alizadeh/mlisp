#include <stdlib.h>
#include <stdio.h>
#include "compile.h"

void compile(mpc_ast_t *t, int level) {
  //Instruction **instructions = malloc(sizeof(*instructions));

  //printf("%*s %s : %s, children : %d\n", level, "", t->tag, t->contents, t->children_num);

  for(int i=0; i < t->children_num; i++) {
    //compile(t->children[i], level + 2);
  }

  if (strstr(t->tag, "sexp|>")) {
    compile(t->children[2], level + 2);
    compile(t->children[3], level + 2);
    compile(t->children[1], level + 2);
  } else {
    for(int i=0; i < t->children_num; i++) {
      compile(t->children[i], level + 2);
    }
  }

  if (strstr(t->tag, "number")) {
    printf("PUSH %d\n", atoi(t->contents));
  }

  if (strstr(t->tag, "sym")) {
    printf("CALL sym %s\n", t->contents);
  }
}

int main() {
  mpc_parser_t *Program  = mpc_new("program");
  mpc_parser_t *Number  = mpc_new("number");
  mpc_parser_t *Sexp  = mpc_new("sexp");
  mpc_parser_t *Exp  = mpc_new("exp");
  mpc_parser_t *Sym  = mpc_new("sym");

  mpca_lang(MPCA_LANG_DEFAULT,
            " program    : <sexp>*; "
            " number     : /[0-9]+/;"
            " sexp       : '(' <exp>* ')'; "
            " exp        : <sexp> | <number> | <sym>; "
            " sym        : '+' | '-' | '*' | '/' ; ",
            Number, Sexp, Exp, Sym, Program, NULL);

  mpc_result_t r;

  char * p = "(+ 10 20) (- 20 (+ 3 3))";

  if (mpc_parse("input", p, Program, &r)) {
    compile(r.output, 0);
    mpc_ast_delete(r.output);
  } else {
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
  }

  mpc_cleanup(5, Number, Sexp, Exp, Sym, Program);
}
