#ifndef __OPENCSCAD_H__
#define __OPENCSCAD_H__

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

#ifndef DEFINE_OPENCSCAD_GLOBALS
#define OPENCSCAD_GLOBAL extern
#else
#define OPENCSCAD_GLOBAL
#endif

struct opencscad_2dpoint {
	double x, y;
};

OPENCSCAD_GLOBAL void cube(double x, double y, double z, int center) ;
OPENCSCAD_GLOBAL void sphere(double r);
OPENCSCAD_GLOBAL void cylinder(double h, double r1, double r2);
OPENCSCAD_GLOBAL void cyl(double h, double r1, double r2, int centered);
OPENCSCAD_GLOBAL void angular_cylinder(double h, double r1, double r2, double fa);
OPENCSCAD_GLOBAL void angular_cyl(double h, double r1, double r2, double fa, int centered);
OPENCSCAD_GLOBAL void onion(void); /* union is reserved word in c. */
OPENCSCAD_GLOBAL void endonion(void);
OPENCSCAD_GLOBAL void diff(void);
OPENCSCAD_GLOBAL void enddiff(void);
OPENCSCAD_GLOBAL void intersection(void);
OPENCSCAD_GLOBAL void endintersection(void);
OPENCSCAD_GLOBAL void xlate(double x, double y, double z);
OPENCSCAD_GLOBAL void endxlate(void);
OPENCSCAD_GLOBAL void scale(double x, double y, double z);
OPENCSCAD_GLOBAL void endscale(void);
OPENCSCAD_GLOBAL void rotate(double angle, double x, double y, double z);
OPENCSCAD_GLOBAL void endrotate(void);

/* 2d primitives */
OPENCSCAD_GLOBAL void square(double x, double y, int center);
OPENCSCAD_GLOBAL void circle(double r);


/* path is an array of pointers to arrays of ints.  The ints are
 * indexes into the point array.  The lists of ints are terminted by
 * the value -1. 
 */
OPENCSCAD_GLOBAL void polygon(struct opencscad_2dpoint point[], int npoints,
			int *path[]);
OPENCSCAD_GLOBAL void module(char *module_name);
OPENCSCAD_GLOBAL void end(void);
OPENCSCAD_GLOBAL void call_module(char *module_name);
OPENCSCAD_GLOBAL void radial_dist(int count, float r);
OPENCSCAD_GLOBAL void end_radial_dist(void);

#undef OPENCSCAD_GLOBAL
#endif


