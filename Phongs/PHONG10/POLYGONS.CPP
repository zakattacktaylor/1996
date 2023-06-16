/* POLYGONS.CPP
*/

#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <iostream.h>
#include <math.h>
#include "types.h"
#include "line.h"
#include "polygons.h"
#include "vector.h"
#include "light.h"
extern "C" {
	#include "modex.h"
}
#include "polytype.def"

vector *p1;
vector *p2;
vector *point;

s16bit d_buff[400];
s16bit u_buff[400];
s16bit d_color[400];
s16bit u_color[400];

s16bit *apoly;
s16bit *averages;
s16bit **indexes;
vector **s_points;
s16bit y_min;
s16bit y_max;
u16bit visible_polygons;

void init_polygons(s16bit number)
{
	visible_polygons = 0;
	averages = (s16bit*)calloc(number, sizeof(s16bit));
	if (averages == NULL)
	{
		cout << "not enough memory -> init_polygons\n";
		exit(1);
	}
	indexes = (s16bit**)calloc(number, sizeof(s16bit*));
	if (indexes == NULL)
	{
		cout << "not enough memory -> init_polygons\n";
		exit(1);
	}
	s_points = (vector**)calloc(number, sizeof(vector*));
	if (s_points == NULL)
	{
		cout << "not enough memory -> init_polygons\n";
		exit(1);
	}
}

s16bit distance()
{
	s16bit z,i;
	s16bit first;
	s16bit count;

	apoly += 6;

	count = 1;
	z = 0;
	first = *apoly;
	z += point[*apoly].z;
	apoly++;
	if (z <= 0)
		goto hidden;

	for (i = 1; i < 255; i++)
	{
		if (*apoly == first)
		{
			apoly++;
			return z / count;
		}
		if (point[*apoly].z <= 0)
			goto hidden;
		z += point[*apoly].z;
		count++;
		apoly++;
	}
	apoly++;
	return z / count;
hidden:
	while (i < 255)
	{
		if (*apoly == first)
		{
			apoly++;
			return -1;
		}
		apoly++;
                i++;
	}
	return -1;
}

inline s16bit check_visible(s16bit x1, s16bit y1, s16bit x2, s16bit y2, s16bit x3, s16bit y3)
{
	s16bit t;
	if (x1 > x2)
	{
		t = x1;
		x1 = x2;
		x2 = x3;
		x3 = t;
		t = y1;
		y1 = y2;
		y2 = y3;
		y3 = t;
	}
	return ((x3-x1)*(y2-y1)-(x2-x1)*(y3-y1));
}

void add_polygons(u16bit polygons, s16bit *polys, vector *r_points)
{
	u16bit i;
	s16bit *save;
	//s16bit v;

	apoly = &polys[0];
	point = &r_points[0];

	for (i = 0; i < polygons; i++)
	{
		save = apoly;
		//v = check_visible(point[poly[0]].x,point[poly[0]].y,point[poly[1]].x,point[poly[1]].y,point[poly[2]].x,point[poly[2]].y);
		//if (v <= 0) continue;

		averages[visible_polygons] = distance();
		if (averages[visible_polygons] > 0)
		{
			indexes[visible_polygons] = save;
			s_points[visible_polygons] = &r_points[0];
			visible_polygons++;
		}
	}
}

void polygon_color(s16bit *poly, vector *point)
{
	u8bit ambient;
	u8bit shades;
	u16bit i;
	s16bit first;
	u8bit flip = no;

	/* skip poly type */
	poly++;

	/* is it inverted? */
	if (*poly == reverse)
		flip = yes;
	poly++;

	/* ambient color */
	ambient = *poly;
	poly++;

	/* shading value */
	shades = *poly;
	poly++;

	/* skip the extra words */
	poly += 2;

	/* get the first point */
	first = *poly;
	intensity(&point[*poly], ambient, shades);
	poly++;

	for (i = 0; i < 255; i++)
	{
		if (*poly == first)
			return;
		intensity(&point[*poly], ambient, shades);
		poly++;
	}
}


void draw_polygons()
{
	u16bit i;
	s16bit *poly;
	vector *point;
	char str[10];

	ltoa(visible_polygons, str, 10);
	print_str(str, 10, 0, 220, 200, 0);

	sort_polygons();

	for (i = 0; i < visible_polygons; i++)
	{
		poly = indexes[i];
		point = s_points[i];
		switch (poly[0])
		{
			case solid_line:
				line_polygon(&poly[6], point, poly[3]);
				break;
			case shaded_line:
				polygon_color(&poly[0], point);
				shaded_line_polygon(&poly[6], point);
				break;
			case gouraud_line:
				polygon_color(&poly[0], point);
				//gouraud_line_polygon(&poly[6], point);
				break;
			case solid:
				fill_polygon(&poly[6], point, poly[3]);
				break;
			case shaded:
				polygon_color(&poly[0], point);
				fill_polygon(&poly[6], point, point[poly[6]].color);
				break;
			case gouraud:
				polygon_color(&poly[0], point);
				gouraud_polygon(&poly[6], point);
				break;
			default:
				break;
		}
	}

	visible_polygons = 0;
}

void sort_polygons()
{
	vector *tmp2;
	s16bit *tmp1;
	s16bit tmp,i,k,j;
	s16bit found=0;

	for (k = 1; k < visible_polygons; k++)
	{
		tmp = averages[k];
		tmp1 = indexes[k];
		tmp2 = s_points[k];
		i=k-1;
		found=0;
		while ((i>=0) &&(!found)) {
			if (tmp > averages[i]) {
				averages[i+1]=averages[i];
				indexes[i+1]=indexes[i];
				s_points[i+1] = s_points[i];
				i--;
			}
			else found=1;
		}
		averages[i+1] = tmp;
		indexes[i+1] = tmp1;
		s_points[i+1] = tmp2;
	}
}

void fill_polygon(s16bit *poly, vector *point, u8bit color)
{
	s16bit i;
	u8bit prev_dir;
	u8bit dir;
	s16bit *a, *b;
	u8bit *ca, *cb;
	u16bit first;

	p1 = &point[0];
	p2 = &point[0];
	y_min = 20000;
	y_max = -20000;
	first = poly[0];

	p1 += poly[0];
	p2 += poly[1];

	if (p1->y < y_min) y_min = p1->y;
	if (p1->y > y_max) y_max = p1->y;

	if (p1->y > p2->y)
		trace_2d(p1->x, p1->y, p2->x, p2->y, &d_buff[0]);
	else
		trace_2d(p1->x, p1->y, p2->x, p2->y, &u_buff[0]);

	p1 -= poly[0];
	p2 -= poly[1];

	for (i = 1; i < 255; i++)
	{
		p1 += poly[i];
		p2 += poly[i+1];

		if (p1->y < y_min) y_min = p1->y;
		if (p1->y > y_max) y_max = p1->y;

		if (p1->y > p2->y)
			trace_2d(p1->x, p1->y, p2->x, p2->y, &d_buff[0]);
		else
			trace_2d(p1->x, p1->y, p2->x, p2->y, &u_buff[0]);

		p1 -= poly[i];
		p2 -= poly[i+1];

		if (poly[i+1] == first) break;
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

void gouraud_polygon(s16bit *poly, vector *point)
{
	s16bit i;
	u8bit prev_dir;
	u8bit dir;
	s16bit *a, *b;
	s16bit *ca, *cb;
	u16bit first;
	s16bit t;

	p1 = &point[0];
	p2 = &point[0];
	y_min = 20000;
	y_max = -20000;
	first = poly[0];

	p1 += poly[0];
	p2 += poly[1];

	if (p1->y < y_min) y_min = p1->y;
	if (p1->y > y_max) y_max = p1->y;

	if (p1->y > p2->y)
	{
		trace_2d(p1->x, p1->y, p2->x, p2->y, &d_buff[0]);
		trace_2d(p1->color, p1->y, p2->color, p2->y, &d_color[0]);
	}
	else
	{
		trace_2d(p1->x, p1->y, p2->x, p2->y, &u_buff[0]);
		trace_2d(p1->color, p1->y, p2->color, p2->y, &u_color[0]);
	}

	p1 -= poly[0];
	p2 -= poly[1];

	for (i = 1; i < 255; i++)
	{
		p1 += poly[i];
		p2 += poly[i+1];

		if (p1->y < y_min) y_min = p1->y;
		if (p1->y > y_max) y_max = p1->y;

		if (p1->y > p2->y)
		{
			trace_2d(p1->x, p1->y, p2->x, p2->y, &d_buff[0]);
			trace_2d(p1->color, p1->y, p2->color, p2->y, &d_color[0]);
		}
		else
		{
			trace_2d(p1->x, p1->y, p2->x, p2->y, &u_buff[0]);
			trace_2d(p1->color, p1->y, p2->color, p2->y, &u_color[0]);
		}

		p1 -= poly[i];
		p2 -= poly[i+1];

		if (poly[i+1] == first) break;
	}

	if (y_min < 0) y_min = 0;
	if (y_min > 399) return;
	if (y_max < 0) return;
	if (y_max > 399) y_max = 399;

	a = &d_buff[y_min];
	b = &u_buff[y_min];
	ca = &d_color[y_min];
	cb = &u_color[y_min];

	for (i = y_min; i <= y_max; i++)
	{

		/*if (*a < 0) *a = 0;
		if (*a > 319) *a = 319;
		if (*b < 0) *b = 0;
		if (*b > 319) *b = 319;

		if (*a > *b)
		{
			t = *a;
			*a = *b;
			*b = t;
			t = *ca;
			*ca = *cb;
			*cb = t;
		}
		*/
		gouraud_block(*ca, *a, *cb, *b, i);
		//set_point(*a,i,*ca);
		//set_point(*b,i,*cb);

		a++;
		b++;
		ca++;
		cb++;
	}
}

void line_polygon(s16bit *poly, vector *point, u8bit color)
{
	s16bit i;
	u16bit first;

	first = poly[0];
	p1 = &point[0];

	for (i = 0; i < 255; i++)
	{
		draw_line(p1[poly[i]].x, p1[poly[i]].y, p1[poly[i+1]].x, p1[poly[i+1]].y, color);
		if (poly[i+1] == first) return;
	}
}

void shaded_line_polygon(s16bit *poly, vector *p1)
{
	s16bit i;
	u16bit first;

	first = poly[0];

	for (i = 0; i < 255; i++)
	{
		draw_line(p1[poly[i]].x, p1[poly[i]].y, p1[poly[i+1]].x, p1[poly[i+1]].y, p1[poly[i]].color);
		if (poly[i+1] == first) return;
	}
}
