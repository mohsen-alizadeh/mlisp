all: y.tab.c lex.yy.c
	gcc -o mlisp y.tab.c lex.yy.c ast.c eval.c

y.tab.c: mlisp.y
	yacc -d mlisp.y

lex.yy.c: y.tab.c mlisp.l
	lex mlisp.l

vm: vm.c vm.h
	gcc -o vm vm.c

clean:
	rm -rf mlisp
	rm -rf y.tab.c
	rm -rf y.tab.h
	rm -rf lex.yy.c
