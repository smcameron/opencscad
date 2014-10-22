#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <getopt.h>

#include "opencscad.h"

void finger(float length, float angle, int segments, float thickness, float taper, float curl, int kink)
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
		if (kink && i == segments / 2)
			a = a + curve * 10;
		else
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

	onion();
	for (i = 0; i < nfingers; i++) {
		finger(length + (i == 0) * length * 0.25, angle, segments, thickness, taper, curl, 0);
		angle += deltaangle;
		length -= deltalength;
	}
	finger(armlength, angle, segments, thickness * 1.5, 2.0 - taper, curl, 1);
	endonion();
	if (!left)
		endrotate();
}

static void usage(void)
{
	fprintf(stderr, "usage: batwing [options]\n");
	fprintf(stderr, "options:\n");
	fprintf(stderr, "-a, --angle          angle between batwing fingers, default is 30 degrees\n");
	fprintf(stderr, "-b, --buildplatform  include a makerbot replicator2 sized build platform in model\n");
	fprintf(stderr, "                     in order to help judge if the model will fit.\n");
	fprintf(stderr, "-c, --curl           Amount of curl in batwing fingers, default is 1.3\n");
	fprintf(stderr, "-l, --left           0, or 1, whether wing should be right or left handed\n");
	fprintf(stderr, "-p, --pair           print a pair of batwings, left and right\n");
	fprintf(stderr, "-s, --segments       segments in batwing fingers, default is 10\n");
	fprintf(stderr, "-t, --thickness      thickness of batwing fingers, default is 8.0\n");
	fprintf(stderr, "-T, --taper          taper of batwing fingers, default is 0.95\n");
	fprintf(stderr, "-z, --size           size of batwing, default is 800\n");
	exit(1);
}

static int parse_int_option(char *o, int *i)
{
	int rc;
 
	rc = sscanf(o, "%d", i);
	if (rc != 1)
		return 1;
	return 0;
}

static int parse_float_option(char *o, float *f)
{
	int rc;

	rc = sscanf(o, "%f", f);
	if (rc != 1)
		return 1;
	return 0;
}

static void process_options(int argc, char *argv[], float *length, int *segments, float *deltaangle,
		int *left, float *thickness, float *taper, float *curl, int *build_platform, int *pair)
{
	int i, c;
	int digit_optind = 0;
	int optind;
	float f;

	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"buildplatform", 0, 0, 'b' },
			{"pair", 0, 0, 'p' },
			{"segments",	required_argument, 0, 's' },
			{"angle",	required_argument, 0,  'a' },
			{"curl",	required_argument, 0,  'c' },
			{"left",	required_argument, 0,  'l' },
			{"size",	required_argument, 0,  'z' },
			{"thickness",	required_argument, 0,  't' },
			{"taper",	required_argument, 0,  'T' },
			{0,         0,                 0,  0 }
		};

		c = getopt_long(argc, argv, "a:bc:l:ps:t:T:z:", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		case 'p':
			*pair = 1;
			break;
		case 'b':
			*build_platform = 1;
			break;
		case 'a':
			if (parse_float_option(optarg, &f))
				usage();
			*deltaangle = f;
			break;
		case 's':
			if (parse_int_option(optarg, &i))
				usage();
			*segments = i;
			break;
		case 'l':
			*left = i;
			break;
		case 'T':
			if (parse_float_option(optarg, &f))
				usage();
			*taper = f;
			break;
		case 'z':
			if (parse_float_option(optarg, &f))
				usage();
			*length = f;
			break;
		case 't':
			if (parse_float_option(optarg, &f))
				usage();
			*thickness = f;
			break;
		case 'c':
			if (parse_float_option(optarg, &f))
				usage();
			*curl = f;
			break;
		default:
			usage();
			break;
		}
	}
}

void replicator_build_platform(int build_platform)
{
	if (!build_platform)
		return;
	scadinline("module replicator2_build_platform()\n");
	scadinline("{\n");
	scadinline("	rotate(v = [0, 0, 1], a = 90)\n");
	scadinline("		translate(v = [0, 0, -5])\n");
	scadinline("		cube(size = [150, 280, 10], center = true);\n");
	scadinline("}\n");

	scadinline("replicator2_build_platform();\n");
}

int main(int argc, char *argv[])
{
	int segments = 10;
	float deltaangle = 30;
	float angle = 0;
	float curl = 1.3;
	float taper = 0.95;
	float length = 80.0;
	float deltalength = length / 8;
	float thickness = 8.0 / 10.0;
	int left = 0;
	int build_platform = 0;
	int pair = 0;

	process_options(argc, argv, &length, &segments, &deltaangle, &left, &thickness,
				&taper, &curl, &build_platform, &pair);

	opencscad_init();
	scadinline("$fn = 16;\n");

	replicator_build_platform(build_platform);

	diff();
	onion();
	fingers(4, length, deltalength, angle, deltaangle, left, segments,
			thickness, taper, curl);

	if (pair) {
		scadinline("translate(v = [%d, %d, 0]) {\n", 0 * (int) length / 2, (int) length / 5);
		fprintf(stderr, "blah\n");
		fingers(4, length, deltalength, angle, deltaangle, !left, segments,
				thickness, taper, curl);
		scadinline("}\n");
	}
	endonion();
	xlate(-length * 2.5, -length * 2.5, length * -5);
	cube(length * 5, length * 5, length * 5, 0);
	endxlate();
	enddiff();

	finalize();
	return 0;
}

