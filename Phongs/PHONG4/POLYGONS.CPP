/* POLYGONS.CPP
*/

#include "types.h"
#include "line.h"
#include "polygons.h"
extern "C" {
	#include "modex.h"
}

#define x_size 20
#define y_size 20

extern s16vector *pt;
extern u8bit color;

s16bit d_buff[400];
s16bit u_buff[400];

const s16bit polygons = (x_size-1) * (y_size-1);
s16bit *poly;
s16bit polys[polygons * 5];
s16bit averages[polygons];
s16bit indexes[polygons];

s16bit distance()
{
	s16bit i;
	s16bit z = 0;

	for (i = 0; i < 4; i++)
	{
		z += pt[*poly].z;
		poly++;
	}

	return z >> 2;
}

void init_polygons()
{
	s16bit i,j;

	poly = &polys[0];

	for (i = 0; i < x_size-1; i++)
		for (j = 0; j < y_size-1; j++)
		{
			*poly = (j*x_size)+i;
			poly++;
			*poly = (j*x_size)+i+1;
			poly++;
			*poly = ((j+1)*x_size)+i;
			poly++;
			*poly = ((j+1)*x_size)+i+1;
			poly++;
			*poly = (j*x_size)+i;
			poly++;
		}
}

void draw_polygons()
{
	s16bit i;

	s16bit *index;
	s16bit *depth;

	poly = &polys[0];
	depth = &averages[0];
	index = &indexes[0];

	for (i = 0; i < polygons; i++)
	{
		*index = i;
		*depth = distance();
		index++;
		depth++;
	}

	sort_polygons();

	for (i = 0; i < polygons; i++)
	{
		poly = &polys[indexes[i]];
		fill_polygon();
	}
}

void sort_polygons()
{
	s16bit tmp,tmp1,i,k,j;
	s16bit found=0;

	for (k = 1; k < polygons; k++)
	{
		tmp=averages[k];
		tmp1=indexes[k];
		i=k-1;
		found=0;
		while ((i>=0) &&(!found)) {
			if (tmp < averages[i]) {
				averages[i+1]=averages[i];
				indexes[i+1]=indexes[i];
				i--;
			}
			else found=1;
		}
		averages[i+1]=tmp;
		indexes[i+1]=tmp1;
	}
}

void fill_polygon()
{
	s16bit i;
	u8bit prev_dir;
	u8bit dir;
	s16bit y_min;
	s16bit y_max;
	s16bit *a, *b;

	if (pt[poly[0]].y > pt[poly[1]].y)
	{
		prev_dir = 0;
		trace_2d(poly[0], poly[1], &d_buff[0]);
	}
	else
	{
		prev_dir = 1;
		trace_2d(poly[0], poly[1], &u_buff[0]);
	}

	for (i = 1; i < 4; i++)
	{
		if (pt[poly[i]].y > pt[poly[i + 1]].y)
		{
			dir = 1;
			/* up to down ie: a max */
			if (dir < prev_dir) y_max = pt[poly[i]].y;
			/* down to up ie: a min */
			else if (dir > prev_dir) y_min = pt[poly[i]].y;
			prev_dir = dir;
			trace_2d(poly[i], poly[i + 1], &d_buff[0]);
		}
		else
		{
			dir = 0;
			/* up to down ie: a max */
			if (dir < prev_dir) y_max = pt[poly[i]].y;
			/* down to up ie: a min */
			else if (dir > prev_dir) y_min = pt[poly[i]].y;
			prev_dir = dir;
			trace_2d(poly[i], poly[i + 1], &u_buff[0]);
		}
	}

	if (y_min < 0) y_min = 0;
	if (y_min > 399) return;
	if (y_max < 0) return;
	if (y_max > 399) y_max = 399;

	a = &d_buff[y_min];
	b = &u_buff[y_min];

	for (i = y_min; i <= y_max; i++)
	{
		if (*a < 0) *a = 0;
		if (*a > 319) *a = 319;
		if (*b < 0) *b = 0;
		if (*b > 319) *b = 319;

		fill_block(*a, i, *b, i, color);

		a++;
		b++;
	}
}