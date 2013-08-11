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

void fivesides(void)
{
	scadinline("$fn = 5;\n");
}

void pentacolumn(double r, double h)
{
	int i;
	double zt;

	diff();
		cylinder(h, r, r);
		cylinder(h, r, r * 0.8);
	enddiff();
	if (r * 0.4 > 3.825) {

		zt = h/r * 40; 

		for (i = 0; i < 5; i++) {
			rotate((360.0 / 5) * i, 0, 0, 1);
				xlate(r, 0, zt);
					pentacolumn(r * 0.4, h * 1.3 - zt);
				endxlate();
			endrotate();
		}
	}
}


int main(int argc, char *argv[])
{
	opencscad_init();
	fivesides();
	onion();
	pentacolumn(150, 50);
	endonion();
	finalize();
	return 0;
}
