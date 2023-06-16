/* VECTOR.CPP
	vectors are {x,y,z}
	functions:
		init_sincos
		set_matrix
		rotate
		depth_emulate
		compare
*/
#include <stdio.h>
#include "types.h"
#include "vector.h"
#include "light.h"
#include "math.h"
extern "C" {
	#include "modex.h"
}
#include "anim_sys.h"

#define size 40

u8bit xan = 0;
u8bit yan = 0;
u8bit zan = 0;
s16bit z_offset = 450;

const s16bit vertices = 8;
s16vector *pt, *pt2;
s16vector points[vertices];
s16vector r_points[vertices];

float x_focus = 320;
float y_focus = 400;

float tsin[256];
float tcos[256];

float mx1, my1, mz1;
float mx2, my2, mz2;
float mx3, my3, mz3;

void init_sincos()
{
	s16bit i;
	float ang;

	for (i = 0; i < 256; i++)
	{
		ang = i / 40.7436642;
		tsin[i] = sin(ang);
		tcos[i] = cos(ang);
	}
	printf("sine/cosine initialized\n");
}

void set_matrix(u8bit *xan, u8bit *yan, u8bit *zan)
{
	mx1 = tsin[*zan] * tsin[*xan] * tsin[*yan] + tcos[*zan] * tcos[*yan];
	my1 = tcos[*xan] * tsin[*yan];
	mz1 = tsin[*zan] * tcos[*yan] - tcos[*zan] * tsin[*xan] * tsin[*yan];

	mx2 = tsin[*zan] * tsin[*xan] * tcos[*yan] - tcos[*zan] * tsin[*yan];
	my2 = tcos[*xan] * tcos[*yan];
	mz2 = -tcos[*zan] * tsin[*xan] * tcos[*yan] - tsin[*zan] * tsin[*yan];

	mx3 = -tsin[*zan] * tcos[*xan];
	my3 = tsin[*xan];
	mz3 = tcos[*zan] * tcos[*xan];
}

void rotate()
{
	pt2->x = pt->x * mx1 + pt->y * my1 + pt->z * mz1;
	pt2->y = pt->x * mx2 + pt->y * my2 + pt->z * mz2;
	pt2->z = pt->x * mx3 + pt->y * my3 + pt->z * mz3;
}

void depth_emulate()
{
	pt2->x = pt2->x * x_focus / pt2->z;
	pt2->y = pt2->y * y_focus / pt2->z;
}

void draw_point(s16vector *p, u8bit *color)
{
	set_point(p->x, p->y, *color);
	//fill_block(pt2->x, pt2->y, pt2->x + 2, pt2->y + 2, *color);
}

void view_adjust()
{
	pt2->x += screen_width / 2;
	pt2->y += screen_height / 2;
}

u8bit on_screen()
{
	if (pt2->x < 0) return 0;
	if (pt2->x > screen_width) return 0;
	if (pt2->y < 0) return 0;
	if (pt2->y > screen_height) return 0;
	return 1;
}

u8bit visible()
{
	float average = 0;

	if (pt2->z-450 <= 0) return 0;
	average += cos(pt2->x * pi / 180 / 91.0222);
	average += cos(pt2->y * pi / 180 / 91.0222);
	average += cos(pt2->z * pi / 180 / 91.0222);
	/* fourth add could be bump map */
	average *= 256/3;
	return average;
}

void init_vectors()
{
	points[0].x = size;
	points[0].y = size;
	points[0].z = size;
	points[1].x = size;
	points[1].y = -size;
	points[1].z = size;
	points[2].x = -size;
	points[2].y = -size;
	points[2].z = size;
	points[3].x = -size;
	points[3].y = size;
	points[3].z = size;
	points[4].x = size;
	points[4].y = size;
	points[4].z = -size;
	points[5].x = size;
	points[5].y = -size;
	points[5].z = -size;
	points[6].x = -size;
	points[6].y = -size;
	points[6].z = -size;
	points[7].x = -size;
	points[7].y = size;
	points[7].z = -size;
}

void update_vectors()
{
	s16bit i;
	u8bit color;

	pt = &points[0];
	pt2 = &r_points[0];

	set_matrix(&xan, &yan, &zan);

	for (i = 0; i < vertices; i++)
	{
		rotate();
		pt2->z += z_offset;
		depth_emulate();
		view_adjust();
		pt++;
		pt2++;
	}
}