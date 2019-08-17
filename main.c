#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <editline/history.h>
#include "mpc.h"

#define MAX_INPUT 1024

static char input[MAX_INPUT];

typedef struct lval {
  int type;
  long num;
  char * err;
  char * sym;
  int count;
  struct lval * cell;

} lval;

enum { LVAL_NUM, LVAL_ERR, LVAL_SYM, LVAL_SEXP };

lval eval(mpc_ast_t *);
lval eval_op(char *, lval, lval);


lval* lval_num(long x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_NUM;
  v->num = x;
  return v;
}

lval* lval_err(char* x) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_ERR;
  v->err = malloc(strlen(x) + 1);
  strcpy(v->err, x);
  return v;
}

lval* lval_sym(char* m) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SYM;
  v->sym = malloc(strlen(x) + 1);
  strcpy(v->sym, m);
  return v;
}

lval* lval_sexp(void) {
  lval* v = malloc(sizeof(lval));
  v->type = LVAL_SEXP;
  v->count = 0;
  v->cell = NULL;
  return v;
}

void lval_del(lval* v) {
  switch(v->type) {
  case LVAL_NUM; break;
  case LVAL_SYM: free(v->sym); break;
  case LVAL_ERR: free(v->err); break;
  case LVAL_SEXP:
    for(int i=0, i < v->count; i++) {
      free(v->cell[i]);
    }
    free(v->cell);
    break;
  }

  free(v);
}


lval* lval_read_num(mpc_ast_t *t) {
  errno = 0;
  long x = strtol(t->contents, NULL, 10);
  return errno == ERANGE ? lval_err("invalid number") : lval_num(x);
}

lval* lval_read(mpc_ast_t* t) {

  /* If Symbol or Number return conversion to that type */
  if (strstr(t->tag, "number")) { return lval_read_num(t); }
  if (strstr(t->tag, "symbol")) { return lval_sym(t->contents); }

  /* If root (>) or sexpr then create empty list */
  lval* x = NULL;
  if (strcmp(t->tag, ">") == 0) { x = lval_sexpr(); }
  if (strstr(t->tag, "sexpr"))  { x = lval_sexpr(); }

  /* Fill this list with any valid expression contained within */
  for (int i = 0; i < t->children_num; i++) {
    if (strcmp(t->children[i]->contents, "(") == 0) { continue; }
    if (strcmp(t->children[i]->contents, ")") == 0) { continue; }
    if (strcmp(t->children[i]->tag,  "regex") == 0) { continue; }
    x = lval_add(x, lval_read(t->children[i]));
  }

  return x;
}

lval* lval_add(lval* v, lval* x) {
  v->count++;
  v->cell = realloc(v->cell, sizeof(lval*) * v->count);
  v->cell[v->count-1] = x;
  return v;
}

void lval_expr_print(lval* v, char open, char close) {
  putchar(open);
  for (int i = 0; i < v->count; i++) {

    /* Print Value contained within */
    lval_print(v->cell[i]);

    /* Don't print trailing space if last element */
    if (i != (v->count-1)) {
      putchar(' ');
    }
  }
  putchar(close);
}

void lval_print(lval x) {
  switch(x.type) {
  case LVAL_NUM: printf("%li\n", x.num); break;

  case LVAL_ERR:
    switch(x.err) {
    case LERR_DIV_ZERO:
      printf("Division by zero error\n"); break;
    case LERR_BAD_OP:
      printf("bad operation error\n"); break;
    }
  }
}

void lval_print(lval* v) {
  switch (v->type) {
  case LVAL_NUM:   printf("%li", v->num); break;
  case LVAL_ERR:   printf("Error: %s", v->err); break;
  case LVAL_SYM:   printf("%s", v->sym); break;
  case LVAL_SEXPR: lval_expr_print(v, '(', ')'); break;
  }
}

void lval_println(lval* v) { lval_print(v); putchar('\n'); }


void eval_program(mpc_ast_t *t) {
  lval res;
  for(int i=0; i < t->children_num; i++) {
    res = eval(t->children[i]);
    lval_print(res);
  }
}

lval eval(mpc_ast_t *t) {
  /*
  printf("-------------\n");
  printf("tag : %s\n", t->tag);
  printf("contents : %s\n", t->contents);
  printf("children : %d\n", t->children_num);
  */

  if (strstr(t->tag, "number")) {
    return lval_num(atoi(t->contents));
  }

  char * op = t->children[1]->contents;

  lval x = eval(t->children[2]);

  for(int i=3; i < t->children_num -1; i++) {
    x = eval_op(op, x, eval(t->children[i]));
  }

  return x;
}

lval eval_op(char * op, lval a, lval b) {
  if (a.type == LVAL_ERR) { return a; }
  if (b.type == LVAL_ERR) { return b; }


  if (strcmp(op, "+") == 0) { return lval_num(a.num + b.num); }
  if (strcmp(op, "-") == 0) { return lval_num(a.num - b.num); }
  if (strcmp(op, "*") == 0) { return lval_num(a.num * b.num); }
  if (strcmp(op, "/") == 0) {
    if (b.num == 0) {
      return lval_err(LERR_DIV_ZERO);
    } else {
      return lval_num(a.num / b.num);
    }
  }


  return lval_err(LERR_BAD_OP);
}

void compile(char * p) {
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

  if (mpc_parse("input", p, Program, &r)) {
    eval_program(r.output);
    mpc_ast_delete(r.output);
  } else {
    mpc_err_print(r.error);
    mpc_err_delete(r.error);
  }

  mpc_cleanup(5, Number, Sexp, Exp, Sym, Program);
}


int main(int argc, char **argv) {
  char *p;

  while((p = readline("mlisp > ")) != NULL) {

    if (strcmp(p,"q") == 0) {
      printf("quit\n");
      break;
    }

    add_history(p);
    compile(p);
    free(p);
  }

  return 0;
}



