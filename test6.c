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

static char *invader1[] = {
	"........",
	"..#.....",
	"...#...#",
	"..######",
	".##.###.",
	"########",
	"#.######",
	"#.#.....",
	"...##.##",
	NULL,
};

static char *invader2[] = {
	"........",
	"#.......",
	"........",
	"#.......",
	"##......",
	"###.....",
	"#.#.....",
	"#.#.....",
	"........",
	NULL,
};
	
static char *s[] = {
	"........",
	".#####..",
	"#.....#.",
	"#.......",
	".#####..",
	"......#.",
	"#.....#.",
	".#####..",
	"........",
	NULL,
};

static char *t[] = {
	"........",
	"#######.",
	"...#....",
	"...#....",
	"...#....",
	"...#....",
	"...#....",
	"...#....",
	"........",
	NULL,
};

static char *e[] = {
	"........",
	"#######.",
	"#.......",
	"#.......",
	"#####...",
	"#.......",
	"#.......",
	"#######.",
	"........",
	NULL,
};

static char *v[] = {
	"........",
	"#.....#.",
	"#.....#.",
	"#.....#.",
	".#...#..",
	".#...#..",
	"..#.#...",
	"...#....",
	"........",
	NULL,
};

static char *underscore[] = {
	"........",
	"........",
	"........",
	"........",
	"........",
	"........",
	"........",
	"........",
	"#######.",
	NULL,
};

static char *kern[] = {
	".",
	".",
	".",
	".",
	".",
	".",
	".",
	".",
	".",
	NULL,
};

static char **STEVE[] = { kern, invader1, invader2, s, t, e, v, e, underscore, NULL };

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
					square(PIXELWIDTH * 0.35, 1, 1);
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

static void renderstring(double x, double y, char **s[])
{
	int i;

	onion();
		renderchar(x, y, s[0]);
		x += PIXELWIDTH;
		for (i = 1; s[i] != NULL; i++) {
			renderchar(x, y, s[i]);
			x += PIXELWIDTH * 8;
		}
	endonion();
}

int main(int argc, char *argv[])
{
	renderstring(0, 0, STEVE);
	return 0;
}
