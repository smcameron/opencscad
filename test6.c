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

#define PIXELWIDTH 6 /* mm */
#define PIXELHEIGHT 8 /* mm */

static char *s[] = {
	".#####.",
	"#.....#",
	"#......",
	".#####.",
	"......#",
	"#.....#",
	".#####.",
	NULL,
};

static char *t[] = {
	"#######",
	"...#...",
	"...#...",
	"...#...",
	"...#...",
	"...#...",
	"...#...",
	NULL,
};

static char *e[] = {
	"#######",
	"#......",
	"#......",
	"#####..",
	"#......",
	"#......",
	"#######",
	NULL,
};

static char *v[] = {
	"#.....#",
	"#.....#",
	"#.....#",
	".#...#.",
	".#...#.",
	"..#.#..",
	"...#...",
	NULL,
};

static void renderchar(double x, double y, char *c[])
{
	int i;
	double maxx;
	double px, py;
	char *p;
	
	onion();

	py = PIXELHEIGHT * 7;
	px = 0; 

	xlate(x, y, 0);
	for (i = 0; c[i] != NULL; i++) {
		px = 0;
		for (p = c[i]; *p; p++) {
			xlate(px, py - (PIXELHEIGHT - 1), 0);
				xlate(0, -PIXELHEIGHT / 2.0 + 1, 0);
					square(PIXELWIDTH * 2, 1, 1);
				endxlate();
				if (*p == '#')
					square(PIXELWIDTH - 2, PIXELHEIGHT - 2, 1);
			endxlate();
			px += PIXELWIDTH;
		}
		py -= PIXELHEIGHT;
	}
	endxlate();
	endonion();
} 
	
static void steve(void)
{
	double x, y;

	x = 0;
	y = 0;

	onion();
	renderchar(x, y, s); x += PIXELWIDTH * 8;
	renderchar(x, y, t); x += PIXELWIDTH * 8;
	renderchar(x, y, e); x += PIXELWIDTH * 8;
	renderchar(x, y, v); x += PIXELWIDTH * 8;
	renderchar(x, y, e); x += PIXELWIDTH * 8;
	endonion();
}

int main(int argc, char *argv[])
{
	steve();
}
