#ifndef vm_h
#define vm_h

void fn_sum();
void fn_substract();
void fn_multiply();
void fn_divide();

char * instruction_name(int type);
void print_stack();

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

#endif
