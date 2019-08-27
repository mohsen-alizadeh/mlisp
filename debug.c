#include "debug.h"

void print_ast(NODE node, int level) {
  printf("%*s %s \n", level * 2, "", node_type(node->node_type));

  if (node->node_type == NODE_FUNC) {
    print_ast(node->u.children.left, level + 1);
    print_ast(node->u.children.right, level + 1);
  }
}

char * node_type(enum NODE_TYPE node_type) {
#define TYPE(a) case a: return #a

  switch(node_type) {
    TYPE(NODE_NUM);
    TYPE(NODE_ERR);
    TYPE(NODE_SYM);
    TYPE(NODE_SEXP);
    TYPE(NODE_FUNC);
  }

  return "UNKNOWN";

#undef TYPE
}
