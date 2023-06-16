/* SHAPE.CPP
*/

#include <stdlib.h>
#include <dos.h>
#include <iostream.h>
#include "types.h"
#include "shape.h"
#include "vector.h"
#include "polygons.h"

shape *shapes;
u16bit shape_count;
u16bit shape_max;

void init_shapes(u16bit number)
{
	shape_count = 0;
	shape_max = number;
	shapes = (shape*)calloc(number, sizeof(shape));
	if (shapes == NULL)
	{
		cout << "not enough memory -> init_shapes\n";
		exit(1);
	}
}

shape *new_shape(u16bit vert, u16bit faces, vector *pdata, s16bit *data, s16bit x, s16bit y, s16bit z)
{
	shape *s;

	if (shape_count == shape_max) return NULL;

	s = &shapes[shape_count];
	if (s == NULL)
	{
		cout << "not enough memory -> init_shapes\n";
		exit(1);
	}
	s->vertices = vert;
	s->r_points = (vector*)calloc(vert, sizeof(vector));
	if (s->r_points == NULL)
	{
		cout << "not enough memory -> init_vectors\n";
		exit(1);
	}
	s->points = pdata;
	s->polygons = faces;
	s->polys = data;
	s->x = x;
	s->y = y;
	s->z = z;

	shape_count++;
	return s;
}

void draw_shapes()
{
	u16bit i;
	shape *s;

	s = &shapes[0];

	for (i = 0; i < shape_count; i++)
	{
		set_matrix(&s->xan, &s->yan, &s->zan);
		update_vectors(s->vertices, &s->points[0], &s->r_points[0], s->x, s->y, s->z);
		add_polygons(s->polygons, &s->polys[0], &s->r_points[0]);
		s++;
	}

	draw_polygons();
}