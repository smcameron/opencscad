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

static void arched_opening(double height, double width, double depth)
{
	xlate(-width / 2.0, -depth / 2.0, -height / 2.0);
	onion();
	cube(width, depth, height - width / 2.0, 0);
	xlate(width / 2.0, depth, height - width / 2.0);
	rotate(90, 1, 0, 0);
	cylinder(depth, width / 2.0, width / 2.0); 
	endrotate();
	endxlate();
	endonion();
	endxlate();
}

void crenelation(double h, double r)
{
	rotate(360.0 / 16.0, 0, 0, 1);
	onion();
		//cube(r * 0.15, r * 3.0, h * 0.12, 1);
		arched_opening(h * 0.35, r * 0.15, r * 3.0);
		rotate(90.0, 0.0, 0.0, 1.0);
			//cube(r * 0.15, r * 3.0, h * 0.12, 1);
			arched_opening(h * 0.35, r * 0.15, r * 3.0);
		endrotate();
		rotate(45.0, 0.0, 0.0, 1.0);
			//cube(r * 0.15, r * 3.0, h * 0.12, 1);
			arched_opening(h * 0.35, r * 0.15, r * 3.0);
		endrotate();
		rotate((45.0 + 90.0), 0.0, 0.0, 1.0);
			//cube(r * 0.15, r * 3.0, h * 0.12, 1);
			arched_opening(h * 0.35, r * 0.15, r * 3.0);
		endrotate();
	endonion();
	endrotate();
}

static void add_windows(double tower_radius, double tower_height)
{
	int i, nwindows;
	double h;
	double angle;

	nwindows = irandomn(3);

	for (i = 0; i < nwindows; i++) {
		h = randomn(tower_height / 4.0) + tower_height * 0.5;
		angle = (double) randomn(360.0);
		xlate(0, 0, h);
		rotate(angle, 0, 0, 1);
		arched_opening(5, 2, tower_radius * 3);	
		endrotate();
		endxlate();
	}
}

static void angular_tower(double x, double y, double r, double h, int flying, double fa)
{
	xlate(x, y, 0);

	/* prevent > 45 degree overhangs to maintain 3d-printability */
	if (r > h * 0.20)
		flying = 0;

	if (!flying)
		diff();
	onion();
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
		xlate(0, 0, 0.7 * r);
		crenelation(2.3 * r, r * 2.0);
	}
	endxlate();
	enddiff();
	endxlate();
	endxlate();
	endonion();
	if (!flying) {
		add_windows(r, h);
		enddiff();
	}
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

	diff();
	onion();
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
	endonion();
	if (!flying) {
		add_windows(r, h);
	}
	if (!flying)
		xlate(0, 0, h + 3);
	else
		xlate(0, 0, h * 0.85);
	crenelation(r * 1.2, r * 2);
	endxlate();
	enddiff();
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

void gothic_arch(double width, double height, double depth)
{
	double radius;
	double archheight;
	double cubeheight;

	radius = 5.0 * width / 4.0; 
	archheight = width;
	cubeheight = height - archheight;

	xlate(-depth / 2.0, 0, cubeheight);
	onion();
	intersection();
	xlate(0, 3.0 * width / 4.0, 0);
	rotate(90, 0, 1, 0);
	cylinder(depth, radius, radius);
	endrotate();
	endxlate();
	xlate(0, -3.0 * width / 4.0, 0);
	rotate(90, 0, 1, 0);
	cylinder(depth, radius, radius);
	endrotate();
	endxlate();
	endintersection();

	xlate(depth / 2.0, 0, -cubeheight / 2.0);
	cube(depth, width, cubeheight, 1);
	endxlate();
	endonion();
	endxlate();
}

static void gothic_arch_array(double length, double width, double height,
				double border_thickness, int narches)
{
	int i;
	double l = length - border_thickness * 2.0;
	double wspan = (l + border_thickness) / narches;
	double wwidth = wspan - border_thickness;
	double x;

	x = -(length / 2.0) + border_thickness + (wwidth / 2.0);

	for (i = 0; i < narches; i++) {
		xlate(x, 0, 0);
		rotate(90, 0, 0, 1);
		gothic_arch(wwidth, height, width);
		endrotate();
		endxlate();
		x += wspan;
	}
}

static void gothic_matrix(double length, double width, double height,
		int larches, int warches, double border_thickness)
{
	onion();
	gothic_arch_array(length, width, height, border_thickness, larches);
	rotate(90, 0, 0, 1);
	gothic_arch_array(width, length, height, border_thickness, warches);
	endrotate();
	endonion();
}

static fancy_roof(double length, double width, double height);

static void gothic_hall(double length, double width, double height,
	int larches, int warches, double border_thickness, int with_roof)
{
	diff();
	xlate(0, 0, (height * 1.05) / 2.0 + border_thickness);
	cube(length * 0.97, width * 0.97, height * 1.05 + border_thickness * 2.0, 1);
	endxlate();
	xlate(0, 0, 1.5 * border_thickness);
	gothic_matrix(length, width, height, larches, warches, border_thickness);
	endxlate();
	enddiff();
	if (with_roof) {
		xlate(0, 0, height * 1.05 + border_thickness * 2.0);
		fancy_roof(width, length, height * 0.7);
		endxlate();
	}
}

static void gabled_roof(double length, double width, double height)
{
	double side;

	side = (height * 2.0) / sqrt(2.0);

	scale(width / (height * 2.0), 1.0, 1.0);
	diff();
	rotate(45, 0, 1, 0);
	cube(side, length, side, 1);
	endrotate();
	xlate(0, 0, -side);  
	cube(side * 2, length * 2, side * 2, 1);
	endxlate();
	enddiff();
	endscale();
}

static fancy_roof(double length, double width, double height)
{
	intersection();
	gabled_roof(length, width, height * 3);
	rotate(90, 0, 0, 1);
	gabled_roof(width, length, height);
	endrotate();
	endintersection();
}

static void english_house_end(double width, double wall_height, double wall_thickness,
			double peak_height, double chimney_height)
{
	rotate(90, 0, 0, 1);
	onion();
	xlate(0, 0, wall_height / 2.0);
	cube(width, wall_thickness, wall_height, 1);
	endxlate();

	xlate(0, 0, wall_height * 0.999);
	gabled_roof(wall_thickness, width, peak_height);
	endxlate();

	xlate(0, 0, (wall_height + chimney_height) / 2.0);
	cube(width * 0.20, wall_thickness, wall_height + chimney_height, 1);
	endxlate();
	endonion();
	endrotate();
}

/* width is y axis, length is x axis */
static void english_house(double width, double length, double height, double peak)
{
	double wall_thickness = width * 0.1;

	if (wall_thickness < 5.0)
		wall_thickness = 5.0;

	diff();
	onion();
	gothic_hall(length * 1.05, width, height, 0, 1, wall_thickness, 0);
	xlate(-length / 2.05, 0, 0);
	english_house_end(width, height * 1.3, wall_thickness, peak, peak * 1.2); 
	endxlate();
	xlate(length / 2.05, 0, 0);
	english_house_end(width, height * 1.3, wall_thickness, peak, peak * 1.2); 
	endxlate();

	xlate(0, 0, height * 1.20);
	rotate(90, 0, 0, 1);
	gabled_roof(length, width, peak);
	endrotate();
	endxlate();
	endonion();
	xlate(0, 0, height * 0.5);
	gothic_arch_array(length, width * 2, height * 0.60 , length * 0.1, 4);
	endxlate();
	enddiff();
}

static void buttress(double width, double length, double height , double angle)
{
	xlate(0, 0, height / 2.0);
	intersection();
	cube(length * 2, width * 2, height, 1);
	rotate(angle, 0, 1, 0);
	cube(length, width, height * 2, 1);
	endrotate();
	endintersection();
	endxlate();
}

static void buttress_array(double width, double length, double height, double angle,
				double spacing, int count)
{
	int i;
	double x;

	x = -(spacing / 2.0) * (count - 1);

	for (i = 0; i < count; i++) {
		xlate(x, 0, 0);
		rotate(90, 0, 0, 1);
		buttress(width, length, height, angle);
		endrotate();
		x += spacing;
		endxlate();
	}
}

static void buttress_array_pair(double width, double length, double height, double angle,
				double spacing, int count, double span_between_arrays)
{
	xlate(0, -span_between_arrays / 2.0, 0);
	buttress_array(width, length, height, angle, spacing, count);
	endxlate();
	rotate(180, 0, 0, 1);
	xlate(0, -span_between_arrays / 2.0, 0);
	buttress_array(width, length, height, angle, spacing, count);
	endxlate();
	endrotate();
}

/* width is yaxis, length is xaxis, h=zaxis, angle 0 = vertical, degrees */
static void buttressed_foundation(double width, double length,
					double height, double buttress_angle,
					double buttress_interval)
{
	onion();
	xlate(0, 0, height / 2.0);
	cube(length, width, height, 1);
	endxlate();
	buttress_array_pair(length * 0.10, width * 0.2, height, buttress_angle,
				(length / 5.0) * 0.9, 5, width * 0.85);
	rotate(90, 0, 0, 1);
	buttress_array_pair(width * 0.05, length * 0.2, height, buttress_angle,
				(width / 5.0) * 0.9, 5, length * 0.85);
	endrotate();
	endonion();
}

static void inverted_buttressed_foundation(double width, double length,
					double height, double buttress_angle,
					double buttress_interval)
{
	xlate(0, 0, height);
	rotate(180, 1, 0, 0);
	buttressed_foundation(width, length, height, buttress_angle, buttress_interval);
	endrotate();
	endxlate();
}

static void keep_foundation(double width, double length, double height) 
{
	if (irandomn(100) < 50) {
		buttressed_foundation(width, length, height, 8, length / 6.0); 
	} else {
		inverted_buttressed_foundation(width, length, height, 8, length / 6.0); 
	}
}

static void keep_topper(double width, double length, double height)
{
	if (irandomn(100) < 50) {
		english_house(width, length, height, height / 2.0);
	} else {
		gothic_hall(length, width, height, 3, 2, length * 0.1, 1);
	}
}

static void keep(double width, double length)
{
	double fheight = hypot(width, length) * 0.7;

	keep_foundation(width, length, fheight);
	xlate(0, 0, fheight);
	keep_topper(width, length, fheight);
	endxlate();
}

static void crenelation_array(double width, double length, double height, double thickness, int n)
{
	int i;
	double x;
	double spacing;

	spacing = (width - thickness * 2) / (double) n;

	printf("/* n = %d */\n", n);
	x = -(spacing * (n - 1)) / 2.0;

	xlate(0, 0, height / 2.0);
	for (i = 0; i < n; i++) {
		xlate(x, 0, 0);
		rotate(90, 0, 0, 1);
		cube(length * 1.1, thickness, height, 1);
		endrotate();
		endxlate();
		x += spacing;
	}
	endxlate();
}

static void crenelation_matrix(double width, double length, double height, double thickness)
{
	onion();
	crenelation_array(length, width, height, thickness / 2.0,
				(int) (length / thickness)); 
	rotate(90, 0, 0, 1);
	crenelation_array(width, length, height, thickness / 2.0,
				(int) (width / thickness)); 
	endrotate();
	endonion();
}

static void crenelated_rectangle(double width, double length,
				double height, double crenheight, double wall_thickness)
{
	double spacing = wall_thickness * 2;

	diff();
	xlate(0, 0, height / 2.0);
	diff();
	cube(width, length, height, 1);
	cube(width - 2 * wall_thickness, length - 2 * wall_thickness, height + wall_thickness, 1);
	enddiff();
	endxlate();
	xlate(0, 0, height - crenheight * 0.95);
	crenelation_matrix(length, width, crenheight, wall_thickness);
	endxlate();
	enddiff();
}

int main(int argc, char *argv[])
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);
	enclosure(8, 70.0, 90.0 * HEIGHT_RADIUS);
	enclosure(6, 38.0, 140.0 * HEIGHT_RADIUS);
	enclosure(4, 25.0, 180.0 * HEIGHT_RADIUS);
	cylinder(3, 10, 20);
	return 0;
}

