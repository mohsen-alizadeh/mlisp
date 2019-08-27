#include "ast.h"
#include "eval.h"
#include <stdio.h>


NODE eval(NODE node) {
  //printf("node %c\n", node->node_type);
  switch (node->node_type) {
  case '+':
    return func_sum(node);
  case 'N':
    return node;
  }

  return NULL;
}


NODE func_sum(NODE node) {

  int right = eval(node->u.children.right)->u.number;
  int left = eval(node->u.children.right)->u.number;

  node->u.number = left + right;
  node->node_type = NODE_NUM;

  return node;
}
