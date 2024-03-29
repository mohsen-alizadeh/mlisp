#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

BLOCK add_block(BLOCK current_block, NODE ast) {
  BLOCK new_block = (BLOCK) malloc(sizeof(struct top_level_block));

  new_block->root_node = ast;
  new_block->next = NULL;

  if (current_block != NULL) {
    current_block->next = new_block;
  }

  return (BLOCK) new_block;
}

NODE make_ast_node(char * sym, NODE left, NODE right) {
  NODE node = (NODE) malloc(sizeof(struct ast));

  node->node_type = NODE_FUNC;
  node->u.children.left = left;
  node->u.children.right = right;

  return node;
}

NODE make_number_node(int value) {
  NODE node = (NODE) malloc(sizeof(struct ast));

  node->node_type = NODE_NUM;
  node->u.number = value;

  return node;
}
