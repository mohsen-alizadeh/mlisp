#ifndef AST_H
#define AST_H

struct ast {
  int node_type;
  union {
    struct {
      struct ast * right;
      struct ast * left;
      int (*func)(void);
    } children;

    int number;
    char * err;
    char * sym;
  } u;
};

struct top_level_block {
  struct ast * root_node;
  struct top_level_block * next;
};

enum NODE_TYPE { NODE_NUM, NODE_ERR, NODE_SYM, NODE_SEXP, NODE_FUNC};

#define NODE struct ast *
#define BLOCK struct top_level_block *

NODE make_ast_node(char *sym, NODE, NODE);
NODE make_number_node(int value);
BLOCK add_block(BLOCK, NODE);

#endif
