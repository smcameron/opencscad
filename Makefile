
all:	test1 test2 test3 test4 test5 test6 pentafort tree batwing
	
opencscad.o:	opencscad.h opencscad.c
	gcc -g -W -Wall -c opencscad.c

test1:	test1.c opencscad.o
	gcc -o test1 opencscad.o test1.c

castle:	castle.c opencscad.o
	gcc -o castle opencscad.o castle.c -lm

test2:	test2.c opencscad.o
	gcc -o test2 opencscad.o test2.c

test3:	test3.c opencscad.o
	gcc -o test3 opencscad.o test3.c -lm

test4:	test4.c opencscad.o
	gcc -g -o test4 opencscad.o test4.c -lm

test5:	test5.c opencscad.o
	gcc -g -o test5 opencscad.o test5.c -lm

test6:	test6.c opencscad.o
	gcc -g -o test6 opencscad.o test6.c -lm

pentafort:	pentafort.c opencscad.o
	gcc -g -o pentafort opencscad.o pentafort.c -lm

tree:	tree.c opencscad.o
	gcc -g -o tree opencscad.o tree.c -lm

batwing:	batwing.c opencscad.o
	gcc -g -o batwing opencscad.o batwing.c -lm

clean:
	rm -fr opencscad.o test1 test2 test3 test4 test5 test6 pentafort tree



