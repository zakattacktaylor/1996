/* POLYGONS.CPP
*/

#include "types.h"
#include "line.h"
#include "polygons.h"
extern "C" {
	#include "modex.h"
}

extern s16vector *pt;

s16bit d_buff[400];
s16bit u_buff[400];

const s16bit polygons = 2;
s16bit *poly;
s16bit polys[polygons * 5] = {	0,1,2,3,0,
				4,5,6,7,4};
s16bit averages[polygons];
s16bit indexes[polygons];
s16bit y_min;
s16bit y_max;
u16bit visible_polygons;

s16bit distance()
{
	s16bit i;
	s16bit z = 0;

	y_min = 10000;
	y_max = -10000;

	for (i = 0; i < 4; i++)
	{
		z += pt[*poly].z;
		poly++;
	}

	return z >> 2;
}

void init_polygons()
{
}

void update_polygons()
{
	s16bit i;
	u8bit color;
	s16bit *index;
	s16bit *depth;

	poly = &polys[0];
	depth = &averages[0];
	index = &indexes[0];
	visible_polygons = 0;

	for (i = 0; i < polygons; i++)
	{
		*depth = distance();
		if (*depth > 0)
		{
			*index = i;
			index++;
			visible_polygons++;
			depth++;
		}
	}

	sort_polygons();

	for (i = 0; i < visible_polygons; i++)
	{
		poly = &polys[5*indexes[i]];
		color = (i * 30) + 120;
		//fill_polygon(&color);
		line_poly(&color);
	}
}

void sort_polygons()
{
	s16bit tmp,tmp1,i,k,j;
	s16bit found=0;

	for (k = 1; k < visible_polygons; k++)
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

void fill_polygon(u8bit *color)
{
	s16bit i;
	u8bit prev_dir;
	u8bit dir;
	s16bit *a, *b;

	for (i = 0; i < 4; i++)
	{
		if (pt[poly[i]].y < y_min) y_min = pt[poly[i]].y;
		if (pt[poly[i]].y > y_max) y_max = pt[poly[i]].y;

		if (pt[poly[i]].y > pt[poly[i+1]].y)
			trace_2d(poly[i], poly[i+1], &d_buff[0]);
		else
			trace_2d(poly[i], poly[i+1], &u_buff[0]);
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

		fill_block(*a, i, *b, i, *color);

		a++;
		b++;
	}
}

void line_poly(u8bit *color)
{
	s16bit i;

	for (i = 0; i < 4; i++)
	{
		draw_2d(poly[i], poly[i+1], color);
	}
}