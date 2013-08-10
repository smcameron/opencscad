#include <stdio.h>
#include <stdlib.h>

#include "opencscad.h"

static int randn(int n)
{
	return (int) ((float) n * (float) rand() / (float) RAND_MAX);
}

static void cylindrical_thing(float length, float r1, float r2)
{
	onion();
		cylinder(length, r1, r2);
	endonion();
}

static void cylindrical_module(char *modulename, float length, float r1, float r2)
{
	module(modulename);
		cylindrical_thing(length, r1, r2);
	end();
}
	
int main(int argc, char *argv[])
{
	int i;

	cylindrical_module("testthing", 10, 5, 3);
	call_module("testthing");
	for (i = 0; i < 100; i++)
		printf("%d\n", randn(20));
	return 0;
}

