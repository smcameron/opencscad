#include <stdio.h>

#include "opencscad.h"

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
	cylindrical_module("testthing", 10, 5, 3);
	call_module("testthing");
	return 0;
}

