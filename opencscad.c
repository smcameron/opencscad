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
#include <math.h>
#include <string.h>

#define DEFINE_OPENCSCAD_GLOBALS
#include "opencscad.h"
#undef DEFINE_OPENCSCAD_GLOBALS

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
	printf("%s}\n", indentation);
	if (strlen(indentation) == 0)
		printf("\n");
}

static void ind(void)
{
	printf("%s", indentation);
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
	printf("cube(size = [%g, %g, %g], center = %s);\n",
		x, y, z, center ? "true" : "false");
}

void sphere(double r)
{
	fixup(&r);
	ind();
	printf("sphere(r = %g);\n", r);
}

void angular_cyl(double h, double r1, double r2, double fa, int centered)
{
	fixup(&h);
	fixup(&r1);
	fixup(&r2);
	ind();
	if (fa >= 90.0) {
		printf("cylinder(h = %g, r1 = %g, r2 = %g, %s$fn = 4);\n", h, r1, r2,
			centered ? "center = true, " : "");
	} else {
		printf("cylinder(h = %g, r1 = %g, r2 = %g, %s$fa = %g);\n", h, r1, r2,
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
	printf("cylinder(h = %g, r1 = %g, r2 = %g%s);\n", h, r1, r2,
		centered ? ", center = true" : "");
}

void cylinder(double h, double r1, double r2)
{
	cyl(h, r1, r2, 0);
}

void onion(void)
{
	ind();
	printf("union() {\n");
	indent();
}

void endonion(void)
{
	end();
}

void diff(void) 
{
	ind();
	printf("difference() {\n");
	indent();
}

void enddiff(void)
{
	end();
}

void intersection(void) 
{
	printf("intersection() {\n");
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
	printf("translate(v = [%g, %g, %g]) {\n", x, y, z);
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
	printf("scale(v = [%g, %g, %g]) {\n", x, y, z);
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
	printf("rotate(a = %g, v = [%g, %g, %g]) {\n", angle, x, y, z);
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
	printf("square(size = [%g, %G], center = %s);\n",
		x, y, center ? "true" : "false");
}

void circle(double r)
{
	fixup(&r);
	ind();
	printf("circle(r = %g);\n", r);
}

void polygon(struct opencscad_2dpoint point[], int npoints,
			int *path[])
{
	int i, j;

	ind();
	printf("polygon(points = [");
	for (i = 0; i < npoints; i++) {
		printf("[%g, %g]", point[i].x, point[i].y);
		if (i != npoints-1)
			printf(",");
	}
	printf("],\n");
	indent();
	ind();
	printf("paths = [");
	indent();
	for (i = 0; path[i]; i++) {
		ind();
		printf("[");
		for (j = 0; path[i][j] != -1; j++) {
			printf("%d", path[i][j]);
			if (path[i][j+1] != -1)
				printf(", ");
		}
		printf("]");

		if (!path[i+1])
			printf(",");
		printf("\n");
	} 
	unindent();
	ind();
	printf("]);\n");
	unindent();
}

void module(char *module_name)
{
	ind();
	printf("module %s()\n", module_name);
	printf("{\n");
	indent();
}

void call_module(char *module_name)
{
	ind();
	printf("%s();\n", module_name);
}

