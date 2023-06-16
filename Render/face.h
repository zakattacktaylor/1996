#ifndef _FACE_H
#define _FACE_H

#include "types.h"
#include "vector.h"

typedef struct {
	int vertex[3];
	long data;
	tVector n;
	int t[6];
} tFace;

void drawGMFace(tFace *face, tVector vect[], tScalar lVals[]);
void drawTFace(tFace *face, tVector vect[], u8bit texture[]);

void sortFaces(tVector v[]);
void drawAllFaces(tVector v[], tScalar lVals[], u8bit texture[]);
void addFace(tFace *face);
void initFaceHandler(int n);

void drawFace(tFace *face, tVector vect[], u8bit color);
void drawGouraudFace(tFace *face, tVector vect[], tVector vectN[], tVector *light);

#endif

