/* VECTOR.CPP
	vectors are {x,y,z}
	functions:
		init_sincos
		set_matrix
		rotate
		depth_emulate
		compare
*/
#include <math.h>

#include "types.h"
#include "vector.h"
#include "anim_sys.h"

#define sqr(a) ((a)*(a))
#define kPi 3.14159265359

float tsin[256];
float tcos[256];

void initSineCosine()
{
	s16bit i;
	float ang;

	for (i = 0; i < 256; i++)
	{
		ang = (float)(i / 40.7436642);
		tsin[i] = (float)sin(ang);
		tcos[i] = (float)cos(ang);
	}
}

void computeRotMatrix(tMatrix M, u8bit xan, u8bit zan, u8bit yan)
{
	M[m11] = tsin[zan] * tsin[xan] * tsin[yan] + tcos[zan] * tcos[yan];
	M[m12] = tcos[xan] * tsin[yan];
	M[m13] = tsin[zan] * tcos[yan] - tcos[zan] * tsin[xan] * tsin[yan];

	M[m21] = tsin[zan] * tsin[xan] * tcos[yan] - tcos[zan] * tsin[yan];
	M[m22] = tcos[xan] * tcos[yan];
	M[m23] = -tcos[zan] * tsin[xan] * tcos[yan] - tsin[zan] * tsin[yan];

	M[m31] = -tsin[zan] * tcos[xan];
	M[m32] = tsin[xan];
	M[m33] = tcos[zan] * tcos[xan];
}

//
// with tScalars for acurate angles
// slower but better for prescion
//
void computeRotMatrix(tMatrix M, tScalar xan, tScalar zan, tScalar yan)
{
	M[m11] = sin(zan) * sin(xan) * sin(yan) + cos(zan) * cos(yan);
	M[m12] = cos(xan) * sin(yan);
	M[m13] = sin(zan) * cos(yan) - cos(zan) * sin(xan) * sin(yan);

	M[m21] = sin(zan) * sin(xan) * cos(yan) - cos(zan) * sin(yan);
	M[m22] = cos(xan) * cos(yan);
	M[m23] = -cos(zan) * sin(xan) * cos(yan) - sin(zan) * sin(yan);

	M[m31] = -sin(zan) * cos(xan);
	M[m32] = sin(xan);
	M[m33] = cos(zan) * cos(xan);
}

void multiply(tMatrix M, tVector *from, tVector *too)
{
	too->x = from->x * M[m11] + from->y * M[m12] + from->z * M[m13];
	too->y = from->x * M[m21] + from->y * M[m22] + from->z * M[m23];
	too->z = from->x * M[m31] + from->y * M[m32] + from->z * M[m33];
}

tScalar dotProduct(tVector *a, tVector *b)
{
	return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
}

u8bit angle(tVector *a, tVector *b)
{
	tScalar dot;
	
	dot = dotProduct(a, b);
	dot = dot / (length(a) * length(b));
	
	dot = (tScalar)acos((tScalar)dot);
	return (u8bit)(255 * dot / kPi);
}

u8bit normalAngle(tVector *a, tVector *b)
{
	tScalar an;

	an = (tScalar)acos(dotProduct(a, b));
	return (u8bit)(255 * an / kPi);
}

tScalar lightValue(tVector *v, tVector *l)
{
	tScalar cs;
	
	cs = -dotProduct(v, l);
	
	if (cs < 0) return 0;
	if (cs > 1) cs = 1;
	return cs;
}

void subtractVectors(tVector *a, tVector *b, tVector *c)
{
	c->x = a->x - b->x;
	c->y = a->y - b->y;
	c->z = a->z - b->z;
}
	
void crossProduct(tVector *a, tVector *b, tVector *c)
{
	c->x = a->y * b->z - a->z * b->y;
	c->y = a->z * b->x - a->x * b->z;
	c->z = a->x * b->y - a->y * b->x;
}

void normalize(tVector *v)
{
	tScalar l = length(v);
	if ((int)l != 0)
		scale(v, 1/l);
}

tScalar length(tVector *v)
{
	return (tScalar)sqrt(sqr(v->x) + sqr(v->y) + sqr(v->z));
}

void scale(tVector *v, tScalar c)
{
	v->x *= c;
	v->y *= c;
	v->z *= c;
}

void depthEmulate(tVector *v)
{
	if ((int)v->z == 0) return;
	v->x = v->x * x_focus / v->z;
	v->y = v->y * y_focus / v->z;
}

void drawPoint(tVector *v, u8bit color)
{
	int y = (int)v->y;
	
	y = (screen_width * y) + (int)v->x;
	out_buff[y] = color;
}

void viewAdjust(tVector *v)
{
	v->x += screen_width / 2;
	v->y += screen_height / 2;
}

boolean onScreen(tVector *v)
{
	if ((v->x < 0) ||
		(v->x > screen_width) ||
		(v->y < 0) ||
		(v->y > screen_height))
		return no;
		
	return yes;
}

boolean onScreen2(tScalar x, tScalar y)
{
	if ((x < 0) ||
		(x > screen_width) ||
		(y < 0) ||
		(y > screen_height))
		return no;
		
	return yes;
}

void multiplyMatrix(tMatrix *a, tMatrix *b, tMatrix *c)
{
}
