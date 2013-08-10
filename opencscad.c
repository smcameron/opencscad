/*
    opencscad : little library to do metaprogramming of OpenSCAD 
    Copyright (C) 2011 Stephen M. Cameron 

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <errno.h>

#define MAX_FILES 1000

static char *fname[MAX_FILES];
static int nfiles = 0;
static FILE *fstack[MAX_FILES];
static int current_file = -1;
#define fss fstack[current_file]

#define DEFINE_OPENCSCAD_GLOBALS
#include "opencscad.h"
#undef DEFINE_OPENCSCAD_GLOBALS

static void pushfile(FILE *f)
{
	current_file++;
	fstack[current_file] = f;
}

static void popfile(void)
{
	fclose(fss);
	current_file--;
}

static void newfile(char *filename)
{
	FILE *f;

	fname[nfiles] = strdup(filename);
	nfiles++;

	/* "x" mode is glibc specific, fopen won't succeed if file already exists. */
	f = fopen(filename, "w+x");
	if (!f) {
		fprintf(stderr, "fopen failed for file '%s': %s\n", filename, strerror(errno));
		exit(1);
	}
	pushfile(f);
}

char indentation[1000] = {0};

static void indent(void)
{
	strcat(indentation, "  ");
}

static void unindent(void)
{
	int n;

	n = strlen(indentation);
	if ((n - 2) >= 0)
		indentation[n - 2] = '\0';
}

void end(void)
{
	unindent();
	fprintf(fss, "%s}\n", indentation);
	if (strlen(indentation) == 0)
		fprintf(fss, "\n");
}

static void ind(void)
{
	fprintf(fss, "%s", indentation);
}

void fixup(double *x)
{
	if (fabsl(*x) < 0.0000001)
		*x = 0.0;
}

void cube(double x, double y, double z, int center)
{
	fixup(&x);
	fixup(&y);
	fixup(&z);
	ind();
	fprintf(fss, "cube(size = [%g, %g, %g], center = %s);\n",
		x, y, z, center ? "true" : "false");
}

void sphere(double r)
{
	fixup(&r);
	ind();
	fprintf(fss, "sphere(r = %g);\n", r);
}

void angular_cyl(double h, double r1, double r2, double fa, int centered)
{
	fixup(&h);
	fixup(&r1);
	fixup(&r2);
	ind();
	if (fa >= 90.0) {
		fprintf(fss, "cylinder(h = %g, r1 = %g, r2 = %g, %s$fn = 4);\n", h, r1, r2,
			centered ? "center = true, " : "");
	} else {
		fprintf(fss, "cylinder(h = %g, r1 = %g, r2 = %g, %s$fa = %g);\n", h, r1, r2,
			centered ? "center = true, " : "", fa);
	}
}

void angular_cylinder(double h, double r1, double r2, double fa)
{
	angular_cyl(h, r1, r2, fa, 0);
}

void cyl(double h, double r1, double r2, int centered)
{
	fixup(&h);
	fixup(&r1);
	fixup(&r2);
	ind();
	fprintf(fss, "cylinder(h = %g, r1 = %g, r2 = %g%s);\n", h, r1, r2,
		centered ? ", center = true" : "");
}

void cylinder(double h, double r1, double r2)
{
	cyl(h, r1, r2, 0);
}

void onion(void)
{
	ind();
	fprintf(fss, "union() {\n");
	indent();
}

void endonion(void)
{
	end();
}

void diff(void) 
{
	ind();
	fprintf(fss, "difference() {\n");
	indent();
}

void enddiff(void)
{
	end();
}

void intersection(void) 
{
	fprintf(fss, "intersection() {\n");
}

void endintersection(void)
{
	end();
}

void xlate(double x, double y, double z)
{
	fixup(&x);
	fixup(&y);
	fixup(&z);
	ind();
	fprintf(fss, "translate(v = [%g, %g, %g]) {\n", x, y, z);
	indent();
}

void endxlate(void)
{
	end();
}

void scale(double x, double y, double z)
{
	fixup(&x);
	fixup(&y);
	fixup(&z);
	ind();
	fprintf(fss, "scale(v = [%g, %g, %g]) {\n", x, y, z);
	indent();
}

void endscale(void)
{
	end();
}

void rotate(double angle, double x, double y, double z)
{
	fixup(&angle);
	fixup(&x);
	fixup(&y);
	fixup(&z);
	ind();
	fprintf(fss, "rotate(a = %g, v = [%g, %g, %g]) {\n", angle, x, y, z);
	indent();
}

void endrotate(void)
{
	end();
}


void square(double x, double y, int center)
{
	fixup(&x);
	fixup(&y);
	ind();
	fprintf(fss, "square(size = [%g, %G], center = %s);\n",
		x, y, center ? "true" : "false");
}

void circle(double r)
{
	fixup(&r);
	ind();
	fprintf(fss, "circle(r = %g);\n", r);
}

void polygon(struct opencscad_2dpoint point[], int npoints,
			int *path[])
{
	int i, j;

	ind();
	fprintf(fss, "polygon(points = [");
	for (i = 0; i < npoints; i++) {
		fprintf(fss, "[%g, %g]", point[i].x, point[i].y);
		if (i != npoints-1)
			fprintf(fss, ",");
	}
	fprintf(fss, "],\n");
	indent();
	ind();
	fprintf(fss, "paths = [");
	indent();
	for (i = 0; path[i]; i++) {
		ind();
		fprintf(fss, "[");
		for (j = 0; path[i][j] != -1; j++) {
			fprintf(fss, "%d", path[i][j]);
			if (path[i][j+1] != -1)
				fprintf(fss, ", ");
		}
		fprintf(fss, "]");

		if (!path[i+1])
			fprintf(fss, ",");
		fprintf(fss, "\n");
	} 
	unindent();
	ind();
	fprintf(fss, "]);\n");
	unindent();
}

void module(char *module_name)
{
	newfile(module_name);
	fprintf(fss, "module %s()\n", module_name);
	fprintf(fss, "{\n");
	indent();
}

void end_module(void)
{
	end();
	popfile();
}

void call_module(char *module_name)
{
	ind();
	fprintf(fss, "%s();\n", module_name);
}

void radial_dist(int count, float r)
{
	float da = 360.0 / (float) count;

	ind();
	fprintf(fss, "for ( i = [ 0 : %d ] ) {\n", count);
	indent();
		ind();
		fprintf(fss, "rotate(a = i * %f, v = [0, 0, 1]) {\n", da);
		indent();
			ind();
			fprintf(fss, "translate(v = [%f, 0, 0]) {\n", r);
			indent();
}

void end_radial_dist(void)
{
	end();
	end();
	end();
}

void opencscad_init(void)
{
	newfile("main_openscad_tmpfile.txt");
}

void finalize(void)
{
	int i;
	char cmd[1000];

	popfile();
	fstack[0] = stdout;
	for (i = 1; i < nfiles; i++) {
		sprintf(cmd, "cat %s", fname[i]);
		system(cmd);
		remove(fname[i]);
	}
	system("cat main_openscad_tmpfile.txt");
	fflush(stdout);
}

