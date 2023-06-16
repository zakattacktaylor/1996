#ifndef _VECTOR_H
#define _VECTOR_H

/************************************
	tVector <x, y, z> interface

	history:
		-12-12-96 ripped old code
		
************************************/

#include "types.h"

// Scalar type for easy resolution increase
typedef float tScalar;

typedef struct {
	tScalar x, y, z;
} tVector;

typedef tScalar tMatrix[9];
enum {m11, m12, m13, m21, m22, m23, m31, m32, m33};

/* new math protos */
void multiply(tMatrix M, tVector *from, tVector *too);
void computeRotMatrix(tMatrix M, u8bit xan, u8bit zan, u8bit yan);
u8bit angle(tVector *a, tVector *b);
u8bit normalAngle(tVector *a, tVector *b);
void normalize(tVector *v);
tScalar length(tVector *v);
void scale(tVector *v, tScalar c);
void depthEmulate(tVector *v);
void initSineCosine(void);
void drawPoint(tVector *v, u8bit color);
void viewAdjust(tVector *v);
boolean onScreen(tVector *v);
boolean isVisible(tVector *v);
void multiplyMatrix(tMatrix *a, tMatrix *b, tMatrix *c);
tScalar lightValue(tVector *v, tVector *l);
tScalar dotProduct(tVector *a, tVector *b);
void crossProduct(tVector *a, tVector *b, tVector *c);
void subtractVectors(tVector *a, tVector *b, tVector *c);

#endif

