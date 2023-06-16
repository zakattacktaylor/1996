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

typedef struct {
	tScalar mx1, my1, mz1, mx2, my2, mz2, mx3, my3, mz3;
} tMatrix;

/* new math protos */
void multiply(tMatrix *M, tVector *from, tVector *too);
void computeRotMatrix(tMatrix *M, u8bit xan, u8bit zan, u8bit yan);
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
void updateVectors(tVector *eyeVector, tVector *eyeLocation, tMatrix *eyeMatrix, tMatrix *M, u16bit vertices, tVector *from, tVector *too, tVector *fromN, tVector *tooN, tScalar x, tScalar y, tScalar z);
boolean isVisible(tVector *v);
void multiplyMatrix(tMatrix *a, tMatrix *b, tMatrix *c);
tScalar lightValue(tVector *v, tVector *l);
tScalar dotProduct(tVector *a, tVector *b);
void crossProduct(tVector *a, tVector *b, tVector *c);
void subtractVectors(tVector *a, tVector *b, tVector *c);

#endif

