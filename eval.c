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
}


NODE func_sum(NODE node) {

  int right = eval(node->u.children.right)->u.value;
  int left = eval(node->u.children.left)->u.value;

  node->u.value = left + right;
  node->node_type = 'N';

  return node;
}
