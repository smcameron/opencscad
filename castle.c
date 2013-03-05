#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>

#include "opencscad.h"

#define THICKNESS_RADIUS (0.08)
#define HEIGHT_RADIUS (0.33)
#define WALL_VERTEX_RANDOMNESS (0.3)
#define TOWER_HEIGHT_RANDOMNESS (0.15)
#define TOWER_RADIUS_RANDOMNESS (0.5)

static double randomn(double n)
{
	return n * ((double) rand() / (double) RAND_MAX);
}

static int irandomn(int n)
{
	return  rand() % n;
}

static double perturb(double value, double amount)
{
	value = value + randomn(value * amount) - (value * amount / 2.0); 
	return value;
}

static double perturbup(double value, double amount)
{
	value = value + randomn(value * amount); 
	return value;
}

void crenelation(double h, double r)
{
	onion();
		cube(r * 0.15, r * 3.0, h * 0.12, 1);
		rotate(90.0, 0.0, 0.0, 1.0);
			cube(r * 0.15, r * 3.0, h * 0.12, 1);
		endrotate();
		rotate(45.0, 0.0, 0.0, 1.0);
			cube(r * 0.15, r * 3.0, h * 0.12, 1);
		endrotate();
		rotate((45.0 + 90.0), 0.0, 0.0, 1.0);
			cube(r * 0.15, r * 3.0, h * 0.12, 1);
		endrotate();
	endonion();
}

static void angular_tower(double x, double y, double r, double h, int flying, double fa)
{
	xlate(x, y, 0);

	/* prevent > 45 degree overhangs to maintain 3d-printability */
	if (r > h * 0.20)
		flying = 0;

	if (flying) {
		angular_cylinder(h * 0.75, r * 0.15, 0.15 * r, fa);
		xlate(0, 0, h * 0.50);
		angular_cylinder(h * 0.25, 0.10 * r, r * 1.2, fa);
		xlate(0, 0, h * 0.25);
		diff();
		angular_cylinder(1.6 * r, 1.2 * r, r * 1.2, fa);
		xlate(0, 0, 0.80 * r);
		angular_cylinder(1.00 * r, 1.0 * r, r * 1.0, fa);
		xlate(0, 0, 0.7 * r);
		crenelation(2.3 * r, r * 2.0);
		endxlate();
	} else {
		angular_cylinder(h, r, 0.80 * r, fa);
		xlate(0, 0, h - 0.6 * r);
		angular_cylinder(0.6 * r, 0.81 * r, r * 1.2, fa);
		xlate(0, 0, r * 0.6);
		diff();
		angular_cylinder(0.6 * r, 1.2 * r, r * 1.2, fa);
		angular_cylinder(0.75 * r, 1.0 * r, r * 1.0, fa);
		xlate(0, 0, 0.5 * r);
		crenelation(2.3 * r, r * 2.0);
	}
	endxlate();
	enddiff();
	endxlate();
	endxlate();
	endxlate();
}

static void round_tower(double x, double y, double r, double h, int flying)
{
	angular_tower(x, y, r, h, flying, 5.0);
}

static void pointy_tower(double x, double y, double r, double h, int flying, double fa)
{
	double pointiness;
	pointiness = perturbup(1.4, 0.5);
	int extra_pointy;

	/* prevent > 45 degree overhangs to maintain 3d-printability */
	if (r > h * 0.20)
		flying = 0;

	extra_pointy = (r < h * 0.30 && irandomn(100) < 40);
	xlate(x, y, 0);
	/* xlate(0, 0, h * 0.8); */
	if (flying) {
		angular_cylinder(h * 0.75, 0.15 * r, 0.15 * r, fa);
		xlate(0, 0, h * 0.5);
		angular_cylinder(h * 0.25, 0.10 * r, r * 1.2, fa);
		xlate(0, 0, h * 0.25);
		angular_cylinder(1.6 * r, 1.2 * r, r * 1.2, fa);
		xlate(0, 0, r * 1.6);
		angular_cylinder(pointiness * r, 1.2 * r, 0, fa);
		if (extra_pointy)
			angular_cylinder(pointiness * 2 * r, 0.6 * r, 0, fa);
	} else {
		angular_cylinder(h, r, 0.80 * r, fa);
		xlate(0, 0, h - 0.6 * r);
		angular_cylinder(0.6 * r, 0.81 * r, r * 1.2, fa);
		xlate(0, 0, r * 0.6);
		angular_cylinder(0.6 * r, 1.2 * r, r * 1.2, fa);
		xlate(0, 0, r * 0.6);
		angular_cylinder(pointiness * r, 1.2 * r, 0, fa);
		if (extra_pointy)
			angular_cylinder(pointiness * 2 * r, 0.6 * r, 0, fa);
	}
	endxlate();
	endxlate();
	endxlate();
	endxlate();
}

static void tower(double x, double y, double r, double h)
{
	double origh = h;
	double origr = r;
	int flying = (irandomn(100) < 30);
	h = perturbup(h, TOWER_HEIGHT_RANDOMNESS);
	r = perturb(r, TOWER_RADIUS_RANDOMNESS);
	switch (irandomn(20)) {
	case 0: angular_tower(x, y, origr, origh, flying, 90.0);
		break;
	case 1:	angular_tower(x, y, r, h, flying, 90.0);
		break;
	case 2: pointy_tower(x, y, r, h, flying, 5.0);
		break;
	case 3: angular_tower(x, y, r, h, flying, 72.0);
		break;
	case 4: angular_tower(x, y, r, h, flying, 90.0);
		break;
	case 5: angular_tower(x, y, r, h, flying, 60.0);
		break;
	case 6: angular_tower(x, y, r, h, flying, 45.0);
		break;
	case 7: pointy_tower(x, y, r, h, flying, 72.0);
		break;
	case 8: pointy_tower(x, y, r, h, flying, 90.0);
		break;
	case 9: pointy_tower(x, y, r, h, flying, 60.0);
		break;
	case 10: pointy_tower(x, y, r, h, flying, 45.0);
		break;
	default:
		round_tower(x, y, r, h, flying );
		break;
	}
}

static void wall(double x1, double y1, double x2, double y2, double thickness, double height)
{
	double angle;
	double dist;
	double clen, ch, c;

	height = perturb(height, 0.1);
	angle = atan2(y2 - y1, x2 - x1);
	printf("/* %lf, %lf, angle = %d */\n", (y2 - y1), (x2 - x1), (int) (angle * 180.0 / M_PI));
	dist = hypot(x2 - x1, y2 - y1);
	xlate(x1, y1, 0);
	rotate(angle * 180 / M_PI, 0, 0, 1);
	/*diff(); */
	cube(dist, thickness, height, 0);
	c = 0.0;
	ch = thickness;
	clen = thickness *1.5;
	while (c < dist) {
		xlate(c, 0, height - thickness);
		cube(clen, ch, ch * 2, 0);
		endxlate();
		c += clen + thickness;
	}
	/* enddiff(); */
	endrotate();
	endxlate();
}

static void enclosure(int npoints, double r, double height)
{
	double thickness = r * THICKNESS_RADIUS;
	double angle = 2.0 * M_PI / (double) npoints;
	double ix, iy, x1, y1, x2, y2, a  = 0;
	double dx, dy;
	int i;

	dx = randomn(r * WALL_VERTEX_RANDOMNESS * 2.0) - r * WALL_VERTEX_RANDOMNESS;
	dy = randomn(r * WALL_VERTEX_RANDOMNESS * 2.0) - r * WALL_VERTEX_RANDOMNESS;
	ix = x1 = cos(a) * r + dx;
	iy = y1 = sin(a) * r + dy;
	for (i = 0; i < npoints - 1; i++) {
		a += angle;
		dx = randomn(r * 0.3) - r * 0.15;
		dy = randomn(r * 0.3) - r * 0.15;
		x2 = cos(a) * r + dx;
		y2 = sin(a) * r + dy;
		wall(x1, y1, x2, y2, thickness, height);
		tower(x1, y1, thickness * 2.0, height * 1.25);
		x1 = x2;
		y1 = y2;
	}
	wall(x1, y1, ix, iy, thickness, height);
	tower(x1, y1, thickness * 2.0, height * 1.25);
}

int main(int argc, char *argv[])
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	enclosure(8, 70.0, 90.0 * HEIGHT_RADIUS);
	enclosure(6, 38.0, 140.0 * HEIGHT_RADIUS);
	enclosure(4, 25.0, 180.0 * HEIGHT_RADIUS);
	cylinder(3, 100, 100);
	return 0;
}

