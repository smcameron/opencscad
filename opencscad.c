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

#define DEFINE_OPENCSCAD_GLOBALS
#include "opencscad.h"
#undef DEFINE_OPENCSCAD_GLOBALS

void cube(double x, double y, double z, int center)
{
	printf("cube(size = [%g, %g, %g], center = %s);\n",
		x, y, z, center ? "true" : "false");
}

void sphere(double r)
{
	printf("sphere(r = %g);\n", r);
}

void cylinder(double h, double r1, double r2)
{
	printf("cylinder(h = %g, r1 = %g, r2 = %g);\n", h, r1, r2);
}

void onion(void)
{
	printf("union() {\n");
}

void endonion(void)
{
	printf("}\n");
}

void diff(void) 
{
	printf("difference() {\n");
}

void enddiff(void)
{
	printf("}\n");
}
void xlate(double x, double y, double z)
{
	printf("translate(v = [%g, %g, %g]) {\n", x, y, z);
}

void endxlate(void)
{
	printf("}\n");
}

void scale(double x, double y, double z)
{
	printf("scale(v = [%g, %g, %g]) {\n", x, y, z);
}

void endscale(void)
{
	printf("}\n");
}

void rotate(double angle, double x, double y, double z)
{
	printf("rotate(a = %g, v = [%g, %g, %g]) {\n", angle, x, y, z);
}

void endrotate(void)
{
	printf("}\n");
}


