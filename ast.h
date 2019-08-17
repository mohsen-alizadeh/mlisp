#ifndef AST_H
#define AST_H

struct ast {
  int node_type;
  union {
    struct {
      struct ast * right;
      struct ast * left;
    } children;

    int value;
  } u;
};

struct top_level_block {
  struct ast * root_node;
  struct top_level_block * next;
};

#define NODE struct ast *
#define BLOCK struct top_level_block *

NODE make_ast_node(int, NODE, NODE);
NODE make_number_node(int value);
BLOCK add_block(BLOCK, NODE);

#endif
