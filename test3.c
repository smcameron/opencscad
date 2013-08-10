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
#include "opencscad.h"
#include "math.h"

#define PI 3.1415927

void castle(double h, double r)
{
	int i;
	double angle, tx, ty;

	diff();
		cylinder(h, r, r * 0.8);
		cylinder(h, 0.5 * r, 0.4 * r);
	enddiff();

	for (i = 0; i < 5; i++) {
		angle = (PI / 180 * 360.0 / 5.0 * i); 
		tx = r * cos(angle);
		ty = r * sin(angle);
		if ((r / 2.2) >= 3.0) {
			xlate(tx, ty, 0);
				castle(h * 1.3, r / 2.2);
			endxlate();
		}
	}
}

int main(int argc, char *argv[])
{
	castle(30, 70);
}
