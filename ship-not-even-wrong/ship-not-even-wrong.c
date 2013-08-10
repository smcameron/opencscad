#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "opencscad.h"

static int randn(int n)
{
	return (int) ((float) n * (float) rand() / (float) RAND_MAX);
}

static float randf(float f)
{
	return ((float) rand() / (float) RAND_MAX) * f;
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

float maxf(float a, float b)
{
	return a > b ? a : b;
}

float minf(float a, float b)
{
	return a > b ? a : b;
}

static void cylinder_ribs(float length, float r1, float r2, int nribs)
{
	static int rib_fn_number = 0;
	int i;
	char ribfn[30], ribsetfn[30];
	float dangle = 360.0 / nribs;
	float angle = 0.0;

	sprintf(ribfn, "ribfn_%d", rib_fn_number++);
	sprintf(ribsetfn, "ribsetfn_%d", rib_fn_number++);
	module(ribfn);
		diff();
			cyl(length, r1, r2, 1);
			xlate(maxf(r1, r2) * 1.05, 0, 0);
				cube(maxf(r1, r2) * 2, maxf(r1, r2) * 2, length + 2, 1);
			end();
			xlate(maxf(r1, r2) * -1.05, 0, 0);
				cube(maxf(r1, r2) * 2, maxf(r1, r2) * 2, length + 2, 1);
			end();
			xlate(0, maxf(r1, r2), 0);
				cube(maxf(r1, r2) * 2.1, maxf(r1, r2) * 2.1, length + 2, 1);
			end();
		end();
	end_module();

	module(ribsetfn);
		onion();
		for (i = 0; i < nribs; i++) {
			rotate(angle, 0, 0, 1); 
				call_module(ribfn);
			end();
			angle += dangle;
		}
		end();
	end_module();
	call_module(ribsetfn);
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
	end_module();
}

static void cylinder_protrusions(float length, float r1, float r2, int nprotrusions)
{
	int i;
	float h, z, x;
	float angle;

	intersection();
		cyl(length, r1 * 1.1, r2 * 1.1, 1);
		onion();
			for (i = 0; i < nprotrusions; i++) {
				h = ((float) (randn(80) + 20) / 100.0) * length;
				z = -((length - h) / 2.0) * ((float) randn(100) / 100.0);
				x = minf(r1, r2) * randf(0.2); 
				angle = randf(360.0);
				xlate(0, 0, z);
					rotate(angle, 0, 0, 1);
						cube(x, maxf(r1, r2) * 3, h, 1);
					end();
				end();
			}
		end();
	end();
}

static void random_cylinder_protrusions(float length, float r1, float r2, int nelaborations)
{
	int nprots = randn(5) + 5;
	float pheight = randf(0.1);
	float l, offset;

	l = (1.0 - randf(0.7)) * length / nelaborations; 
	offset = randf((length / 2.0) - (l / 2.0));
	if (randn(100) < 50)
		offset = -offset;
	xlate(0, 0, offset);
		cylinder_protrusions(l, ((r1 + r2) / 2.0) * (1 + pheight), ((r1 + r2) / 2.0) * (1 + pheight), nprots);
	end();
}

static void random_cylinder_ribs(float length, float r1, float r2, int nelaborations)
{
	int nribs = randn(5) + 5;
	float ribheight = randf(0.3);
	float l, offset;

	l = (1.0 - randf(0.7)) * length / nelaborations; 
	offset = randf((length / 2.0) - (l / 2.0));
	if (randn(100) < 50)
		offset = -offset;
	xlate(0, 0, offset);
		cylinder_ribs(l, r1 * (1 + ribheight), r2 * (1 + ribheight), nribs);
	end();
}

static void random_cylinder_rings(float length, float r1, float r2, int nelaborations)
{
	int nrings = randn(4) + 1;
	float ringheight = randf(0.05);
	float l, offset;

	l = (1.0 - randf(0.7)) * length / nelaborations; 
	offset = randf((length / 2.0) - (l / 2.0));
	if (randn(100) < 50)
		offset = -offset;
	xlate(0, 0, offset);
		cylinder_rings(l, r1 * (1 + ringheight), r2 * (1 + ringheight), nrings);
	end();
}

static void elaborate_cylinder(float length, float r1, float r2, int nelaborations)
{
	int i;

	onion();
		cyl(length, r1, r2, 1);
		for (i = 0; i < nelaborations; i++) {
			int e = randn(3);

			switch (e) {
			case 0: random_cylinder_protrusions(length, r1, r2, nelaborations);
				break;
			case 1: random_cylinder_ribs(length, r1, r2, nelaborations);
				break;
			case 2: random_cylinder_rings(length, r1, r2, nelaborations);
				break;
			}
		}
	end();
}

static void thruster_module(char *modulename, float length, float r1, float r2)
{
	int nelaborations = randn(2);

	module(modulename);
		diff();
			elaborate_cylinder(length, r1, r2, nelaborations);
			xlate(0, 0, length * 0.05);
				cyl(length, r1 * 0.95, r2 * 0.95, 1);
			end();
		end();
	end_module();
}

static void thruster(float length, float r1, float r2)
{
	static int modulenum = 0;
	char modname[100];
	float v = randf(0.3) - 0.15 + 1.0;
	float v2 = randf(0.3) - 0.15 + 1.0;

	sprintf(modname, "thruster_%d", modulenum++);
	thruster_module(modname, length, r1 * v, r2 * v2);
	call_module(modname);
}

static void thruster_cluster(float r)
{
	static int modnum = 0;
	int ttype = randn(3);
	float v1, v2, v3;
	float r1, r2, r3;
	int n;
	char modname[100];


	onion();
		cyl(r * 0.2, r1 = r, r2 = r, 1);
		xlate(0, 0, r * 0.19);
		v1 = randf(0.3) - 0.15 + 1.0;
		v2 = randf(0.3) - 0.15 + 1.0;
		v3 = randf(0.3) - 0.15 + 1.0;
		switch(ttype) {
			case 0: thruster(r * v1  * 1.5, r * v2 * 0.4, r * v3);
				break;
			default:
				n = randn(7) + 2;
				r1 = r * 0.5 * v1;
				r2 = r * 0.2 * v2;
				r3 = r * 0.7;
				sprintf(modname, "thruster_r_%d", modnum++);
				thruster(r1 * v3 * 1.5, r1 * v2 * 0.4, r1 * v3);
				thruster_module(modname, r2 * v3 * 1.5, r2 * v2 * 0.4, r2 * v3); 
				radial_dist(n, r3);
					call_module(modname);
				end_radial_dist();
				break;
			
		}
		end();
	end();
}
	
int main(int argc, char *argv[])
{
	unsigned int seed;
	struct timeval tv;

	gettimeofday(&tv, NULL);

	if (argc > 1) {
		if (sscanf(argv[1], "%u", &seed) != 1) 
			seed = tv.tv_usec;
	} else
		seed = tv.tv_usec;

	srand(seed);

	opencscad_init();
#if 0
	cylindrical_module("testthing", 10, 5, 3);
	call_module("testthing");
#endif
	thruster_cluster(20);
	finalize();
	return 0;
}

