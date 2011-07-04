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
#include <stdlib.h>
#include <math.h>

#include "opencscad.h"

double percent(double n)
{
	return (n/100.0 * rand()) / (double) RAND_MAX;
}

void crenelation(double h, double r)
{
	onion();
		cube(r * 0.25, r * 3.0, h * 0.12, 1);
		rotate(90.0, 0.0, 0.0, 1.0);
			cube(r * 0.25, r * 3.0, h * 0.12, 1);
		endrotate();
		rotate(45.0, 0.0, 0.0, 1.0);
			cube(r * 0.25, r * 3.0, h * 0.12, 1);
		endrotate();
		rotate((45.0 + 90.0), 0.0, 0.0, 1.0);
			cube(r * 0.25, r * 3.0, h * 0.12, 1);
		endrotate();
	endonion();
}

void tower(double r, double h)
{
	int i;
	double h1, r1, angle, nx, ny, nz;

	onion();
		diff();
			cylinder(h, r, r * 0.85);
			cylinder(h, r * 0.85, r * 0.75);
		enddiff();

		xlate(0, 0, h * 0.95);
		diff();
			onion();
			cylinder(h * 0.15, r * 0.85, r * 1.25); 
			xlate(0, 0, h * 0.12); 
				diff();
					cylinder(h * 0.15, r * 1.25, r * 1.25); 
					cylinder(h * 0.15 + 1, r * 0.95, r * 0.95);
				enddiff();
			endxlate();
			endonion();
			xlate(0, 0, 0.25 * h);
				crenelation(h, r);
			endxlate();
		enddiff();
		endxlate();

		for (i = 0; i < 5; i++) {
			angle = (360.0 * rand()) / RAND_MAX * 3.1415927 / 180.0;
			nx = cos(angle) * r * 0.85;
			ny = sin(angle) * r * 0.85;
			nz = 0.25 * h + (percent(55.0) * h);
			r1 = percent(40.0) * r;
			h1 = percent(50.0) * h;

			if (r1 < 5)
				continue;
			if (h1 < 20)
				continue;
			xlate(nx, ny, nz);
				tower(r1, h1);
			endxlate();
		}
		xlate(0, 0, -h / 5.0);
			cylinder(h / 5.0, r * 0.1, r);
		endxlate();
	endonion();
}

int main(int argc, char *argv[])
{
	tower(70.0, 70*5.0);
}
