#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "opencscad.h"

double r(void)
{
        return (double) rand() / (double) RAND_MAX;
}

#define TILTANGLE (25.0 + r() * 10.0)
#define ZANGLE (r() * 360.0)
#define HEIGHTFACT (r() * 0.3 + 0.6)

void tree(int level, double height);
void branch(int level, double tilt, double height, double x, double y, double z)
{
	xlate(0, 0, height);
		rotate(tilt, x, y, z);
			rotate(ZANGLE, 0, 0, 1);
				tree(level, height * HEIGHTFACT);
			endrotate();
		endrotate();
	endxlate();
}

void tree(int level, double height)
{
	cylinder(height, height / 18.0, height / 20.0);
	if (level > 5)
		return;
	branch(level + 1, TILTANGLE, height, 1, 0, 0);
	branch(level + 1, -TILTANGLE, height, 1, 0, 0);
	branch(level + 1, TILTANGLE, height, 0, 1, 0);
	branch(level + 1, -TILTANGLE, height, 0, 1, 0);
}

int main(int argc, char *argv[])
{
	struct timeval tv;
	
	gettimeofday(&tv, NULL);
	srand(tv.tv_usec);

	tree(0, 20.0);
}

