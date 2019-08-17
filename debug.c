#include "ast.h"
#include <stdio.h>

void print_node(NODE node)
{
  switch(node->node_type) {
  case 'N':
    printf("%d", node->u.value); break;
  case '+':
  case '-':
  case '*':
  case '/':
    printf("(%c ", node->node_type);

    print_node(node->u.children.left);

    printf(" ");

    print_node(node->u.children.right);

    printf(")");
    break;

  }
}
