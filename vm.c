#include <stdlib.h>
#include <stdio.h>
#include "vm.h"

#define STACK_SIZE 100
#define PUSH(a) stack[sp++] = a
#define POP() stack[--sp]

typedef struct Instruction {
  int type;
  int operand;
} Instruction;

enum InstructionType {
                  PUSH,
                  POP,
                  CALL,
                  QUIT,
};

Instruction instructions[] = {
                              {PUSH, 10}, // push 10
                              {PUSH, 20}, // push 20
                              {CALL, 0},  // call 1
                              {QUIT, 0},  // QUIT 0
};

int *stack;
int sp = 0;
Instruction *ip;


void (*functions[])() = { fn_sum, fn_substract, fn_multiply, fn_divide };

/* instructions

   push 19
   pop  20
   call 1

 */

void vm_eval_next() {

}

int main() {
  stack = malloc(sizeof(*stack) * STACK_SIZE);
  ip = &instructions[0];

  while(1) {
    printf("%s %d \n", instruction_name(ip->type), ip->operand);

    switch(ip->type) {
    case PUSH:
      PUSH(ip->operand); break;
    case CALL:
      functions[ip->operand]();
      break;
    case QUIT:
      return ip->operand;
    }

    print_stack();
    ip++;
  }

  return 0;
}

void fn_sum() {
  PUSH(POP() + POP());
}

void fn_substract() {
  PUSH(POP() - POP());
}

char * instruction_name(int type) {
#define TYPE(a) case a: return #a
  switch(type) {
    TYPE(PUSH);
    TYPE(POP);
    TYPE(CALL);
    TYPE(QUIT);
  }

#undef TYPE
  return "UNKNOWN";
}

void print_stack() {
  printf("---------- STACK ----------\n");
  for(int i=0; i < sp; i++) {
    printf("%d   %d\n", i, stack[i]);
  }

  printf("---------------------------\n");
}

void fn_multiply() {
  PUSH(POP() * POP());
}

void fn_divide() {
  PUSH(POP() / POP());
}
