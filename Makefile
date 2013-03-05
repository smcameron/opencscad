
all:	test1 test2 test3 test4 test5 test6 pentafort tree
	
opencscad.o:	opencscad.h opencscad.c
	gcc -W -Wall -c opencscad.c

test1:	test1.c opencscad.o
	gcc -o test1 opencscad.o test1.c

castle:	castle.c opencscad.o
	gcc -o castle opencscad.o castle.c -lm

test2:	test2.c opencscad.o
	gcc -o test2 opencscad.o test2.c

test3:	test3.c opencscad.o
	gcc -o test3 -lm opencscad.o test3.c

test4:	test4.c opencscad.o
	gcc -g -o test4 -lm opencscad.o test4.c

test5:	test5.c opencscad.o
	gcc -g -o test5 -lm opencscad.o test5.c

test6:	test6.c opencscad.o
	gcc -g -o test6 -lm opencscad.o test6.c

pentafort:	pentafort.c opencscad.o
	gcc -g -o pentafort -lm opencscad.o pentafort.c

tree:	tree.c opencscad.o
	gcc -g -o tree -lm opencscad.o tree.c


clean:
	rm -fr opencscad.o test1 test2 test3 test4 test5 test6 pentafort tree



