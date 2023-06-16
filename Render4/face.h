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

void drawGMFace(tFace *face, tVector vect[], u8bit texture[]);
void drawTFace(tFace *face, tVector vect[], u8bit texture[], int size);

void sortFaces();
void drawAllFaces();
void addFace(tFace *face, void *shape);
void initFaceHandler(int n);
void killFaceHandler();
void drawFace(tFace *face, tVector vect[], u8bit color);

#endif

