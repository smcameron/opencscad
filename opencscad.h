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

OPENCSCAD_GLOBAL void cube(double x, double y, double z, int center) ;
OPENCSCAD_GLOBAL void sphere(double r);
OPENCSCAD_GLOBAL void cylinder(double h, double r1, double r2);
OPENCSCAD_GLOBAL void onion(void); /* union is reserved word in c. */
OPENCSCAD_GLOBAL void endonion(void);
OPENCSCAD_GLOBAL void diff(void);
OPENCSCAD_GLOBAL void enddiff(void);
OPENCSCAD_GLOBAL void xlate(double x, double y, double z);
OPENCSCAD_GLOBAL void endxlate(void);
OPENCSCAD_GLOBAL void scale(double x, double y, double z);
OPENCSCAD_GLOBAL void endscale(void);
OPENCSCAD_GLOBAL void rotate(double angle, double x, double y, double z);
OPENCSCAD_GLOBAL void endrotate(void);

#undef OPENCSCAD_GLOBAL
#endif


