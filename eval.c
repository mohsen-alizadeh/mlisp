#include "ast.h"
#include "eval.h"
#include <stdio.h>

NODE eval(NODE node) {
  //printf("node %c\n", node->node_type);
  switch (node->node_type) {
  case '+':
  case '-':
  case '/':
  case '*':
    return func_op(node, node->node_type);
  case 'N':
    return node;
  }
}


NODE func_op(NODE node, char op) {

  int right = eval(node->u.children.right)->u.value;
  int left = eval(node->u.children.left)->u.value;

  if (op == '+')
    node->u.value = left + right;
  else if (op == '-')
    node->u.value = left - right;
  else if (op == '/')
    node->u.value = left / right;
  else if (op == '*')
    node->u.value = left * right;

  node->node_type = 'N';

  return node;
}
