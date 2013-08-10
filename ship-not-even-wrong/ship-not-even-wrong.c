#include <stdio.h>
#include <stdlib.h>

#include "opencscad.h"

static int randn(int n)
{
	return (int) ((float) n * (float) rand() / (float) RAND_MAX);
}

static void cylinder_rings(float length, float r1, float r2, int nrings)
{
	int i;
	float ringspacing = length / (nrings + 1);
	float ringheight = ringspacing / 3.0;
	float z, dz, r, dr;

	z = -(ringspacing * nrings) / 2;
	dz = ringspacing;
	r = r1 * 1.3;
	dr = ((r2 * 1.3) - r) / nrings;

	for (i = 0; i < nrings; i++) {
		xlate(0, 0, z);
			cyl(ringheight, r, r, 1);
			z += dz;
			r += dr; 
		end();
	}
}

static void cylindrical_thing(float length, float r1, float r2)
{
	onion();
		cyl(length, r1, r2, 1);
		cylinder_rings(length, r1, r2, randn(3) + 5);
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

