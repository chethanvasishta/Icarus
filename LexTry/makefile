all:
	yacc -d ex1.y && lex ex1.l && cc lex.yy.c y.tab.c -o Ex -lfl
new:
	lex ex1.l && cc -c lex.yy.c -o lex.yy.o && yacc -d ex1.y -o y.tab.cc && g++ lex.yy.o y.tab.cc -o Ex -lfl
