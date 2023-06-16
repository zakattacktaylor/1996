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
#include <stdlib.h>
#include <iostream.h>
#include "types.h"
#include "vector.h"
#include "math.h"
#include "light.h"
extern "C" {
	#include "modex.h"
}
#include "anim_sys.h"

s16bit vertices;
vector *pt, *pt2;
vector *points;
vector *r_points;

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

void f_set_matrix(float *xan, float *yan, float *zan)
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

void rotate(vector *pt, vector *pt2)
{
	pt2->x = pt->x * mx1 + pt->y * my1 + pt->z * mz1;
	pt2->y = pt->x * mx2 + pt->y * my2 + pt->z * mz2;
	pt2->z = pt->x * mx3 + pt->y * my3 + pt->z * mz3;
}

void depth_emulate()
{
	if (pt2->z == 0) return;
	pt2->x = pt2->x * x_focus / pt2->z;
	pt2->y = pt2->y * y_focus / pt2->z;
}

void draw_point(vector *pt)
{
	set_point(pt->x, pt->y, pt->color);
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

	/* haha volume clipping */
	if (pt2->z-450 <= 0) return 0;
	//average += cos(pt2->x * pi / 180 / 91.0222);
	//average += cos(pt2->y * pi / 180 / 91.0222);
	//average += cos(pt2->z * pi / 180 / 91.0222);
	/* fourth add could be bump map */
	average *= 256/3;
	return average;
}

void update_vectors(u16bit vertices, vector *from, vector *too, s16bit x, s16bit y, s16bit z)
{
	s16bit i;
	u8bit color;

	pt = &from[0];
	pt2 = &too[0];

	for (i = 0; i < vertices; i++)
	{
		rotate(pt, pt2);
		pt2->x += x;
		pt2->y += y;
		pt2->z += z;
		depth_emulate();
		view_adjust();
		//draw_point(pt2);
		pt++;
		pt2++;
	}
}

void update_normals(u16bit vertices, vector *from, vector *too)
{
	s16bit i;
	u8bit color;

	pt = &from[0];
	pt2 = &too[0];

	for (i = 0; i < vertices; i++)
	{
		rotate(pt, pt2);
		intensity(pt2);
		pt++;
		pt2++;
	}
}