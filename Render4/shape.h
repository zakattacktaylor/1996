#ifndef _SHAPE_H
#define _SHAPE_H

#include "types.h"
#include "vector.h"
#include "face.h"

typedef struct {
	int vertices;
	tVector *from, *too;
	tVector loc;
	tMatrix M;
	u8bit xan, yan, zan;
	int faces;
	tFace *face;
	u8bit *texture;
	int texture_size;
	int dx, dy;
} tShape;

tShape *new_shape(int vs, int fs);
void update_shape(tShape *shape, tMatrix E, tVector *eloc, tVector *lightPoint);
void bounce_shape(tShape *s);

tShape *new_pyramid();
tShape *new_sphere(tScalar radius, int res, u8bit c1, u8bit c2);

#endif

