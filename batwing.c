#include <stdio.h>
#include <math.h>

#include "opencscad.h"

void finger(float length, float angle, int segments, float thickness, float taper, float curl)
{
	float len = length / (float) segments;
	float a = angle;
	float curve = 1.0;
	float diam = thickness;
	float x, y;
	int i;

	onion();

	x = 0;
	y = 0;
	for (i = 0; i < segments; i++) {
		xlate(x, y, 0);
		rotate(a, 0, 0, 1);
		rotate(90, 0, 1, 0);
		xlate(0, 0, 0);
		sphere(diam * 1.1);
		endxlate();
		xlate(0, 0, len);
		sphere(diam * 1.1);
		endxlate();
		cylinder(len * 1.05, diam, diam * taper);
		endrotate();
		endrotate();
		endxlate();
		x = x + cos(a * M_PI / 180.0) * len;
		y = y + sin(a * M_PI / 180.0) * len;
		a = a + curve;
		curve = curve * curl;
		diam = diam * taper;
	}
	endonion();
}

void fingers(int nfingers, float length, float deltalength, float angle, float deltaangle, int left,
		int segments, float thickness, float taper, float curl)
{
	int i;
	float armlength = 0.7 * length;

	if (!left)
		rotate(180, 1, 0, 0);

	diff();
	onion();
	for (i = 0; i < nfingers; i++) {
		finger(length, angle, segments, thickness, taper, curl);
		angle += deltaangle;
		length -= deltalength;
	}
	finger(armlength, angle, segments, thickness * 1.5, 2.0 - taper, curl);
	endonion();
	xlate(-length * 2.5, -length * 2.5, left * length * -5);
	cube(length * 5, length * 5, length * 5, 0);
	endxlate();
	enddiff();
	if (!left)
		endrotate();
}

int main(int argc, char *argv[])
{
	int segments = 10;
	float deltaangle = 30;
	float angle = 0;
	float curl = 1.3;
	float taper = 0.95;
	float length = 800;
	float deltalength = length / 8;
	float thickness = 8.0;
	int left = 0;
	opencscad_init();
	scadinline("$fn = 16;\n");
	fingers(4, length, deltalength, angle, deltaangle, left, segments,
			thickness, taper, curl);
	finalize();
	return 0;
}

