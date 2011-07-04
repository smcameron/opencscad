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
#include <stdio.h>
#include <string.h>

#include "opencscad.h"

#define DIM 32
static int dim = DIM;
static int height[DIM * DIM];

int idx(int x, int y)
{
	return y * dim + x;
}

void swap(int *x, int *y)
{
	int tmp;

	tmp = *x;
	*x = *y;
	*y = tmp;
}

void terrain(int *h, int x1, int y1, int x2, int y2)
{
	int midx, midy, x, y;
	double nh, r, factor, oldheight;

	if (y2 < y1)
		swap(&y1, &y2);
	if (x2 < x1)
		swap(&x1, &x2);

	if (x2 - x1 < 2)
		return;
	if (y2 - y1 < 2)
		return;

	midx = x1 + (x2 - x1) / 2; 
	midy = y1 + (y2 - y1) / 2; 

	factor = (x2 - x1) / 2.0;

	oldheight = (double) h[idx(x1, y1)];
	oldheight += (double) h[idx(x1, y2)];
	oldheight += (double) h[idx(x2, y1)];
	oldheight += (double) h[idx(x2, y2)];
	oldheight /= 4.0;


	r = 0.0 + rand() / (0.0 + RAND_MAX) - 0.5;
	nh = (oldheight + factor * r);
	for (x = x1; x < x2; x++) {
		for (y = y1; y < y2; y++) {
			h[idx(x, y)] = nh;
		}
	}
	terrain(h, x1, y1, midx, midy);
	terrain(h, midx, y1, x2, midy);
	terrain(h, x1, midy, midx, y2);
	terrain(h, midx, midy, x2, y2);
}

int main(int argc, char *argv[])
{
	int x, y;

	memset(height, 0, sizeof(height));
	terrain(height, 0, 0, dim - 1, dim - 1);

	for (y = 0; y < dim; y++) {
		for (x = 0; x < dim; x++) {
			xlate(x, y, 0);
				cube(1.05, 1.05, (double) height[idx(x, y)], 0);
			endxlate();
		}
	}
}
