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
extern "C" {
	#include "modex.h"
}

#define sqr(a) ((a)*(a))
#define kPi 3.14159265359

float tsin[256];
float tcos[256];
tVector notVisible[] = {1, 1, 1};

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

void computeRotMatrix(tMatrix *M, u8bit xan, u8bit zan, u8bit yan)
{
	M->mx1 = tsin[zan] * tsin[xan] * tsin[yan] + tcos[zan] * tcos[yan];
	M->my1 = tcos[xan] * tsin[yan];
	M->mz1 = tsin[zan] * tcos[yan] - tcos[zan] * tsin[xan] * tsin[yan];

	M->mx2 = tsin[zan] * tsin[xan] * tcos[yan] - tcos[zan] * tsin[yan];
	M->my2 = tcos[xan] * tcos[yan];
	M->mz2 = -tcos[zan] * tsin[xan] * tcos[yan] - tsin[zan] * tsin[yan];

	M->mx3 = -tsin[zan] * tcos[xan];
	M->my3 = tsin[xan];
	M->mz3 = tcos[zan] * tcos[xan];
}

void multiply(tMatrix *M, tVector *from, tVector *too)
{
	too->x = from->x * M->mx1 + from->y * M->my1 + from->z * M->mz1;
	too->y = from->x * M->mx2 + from->y * M->my2 + from->z * M->mz2;
	too->z = from->x * M->mx3 + from->y * M->my3 + from->z * M->mz3;
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
	return (255 * dot / kPi);
}

u8bit normalAngle(tVector *a, tVector *b)
{
	tScalar an;

	an = (tScalar)acos(dotProduct(a, b));
	return (255 * an / kPi);
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
	set_point((int)v->x, (int)v->y, color);
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

void drawLine(tScalar x1, tScalar y1, tScalar x2, tScalar y2, u8bit color)
{
	tScalar dx, dy;
	int l;
	tScalar x, y;
	
	dx = x2 - x1;
	dy = y2 - y1;
	
	if (dx > dy)
		l = (int)dx;
	else
		l = (int)dy;
		
	if (l < 0)
		l = -l;
	if (l == 0) return;
		
	dx = dx / l;
	dy = dy / l;
	
	x = x1;
	y = y1;
	while (l > 0) {
		if (onScreen2(x, y) == yes)
			set_point((int)x, (int)y, color);
			
		x += dx;
		y += dy;
		l--;
	}
}		

void multiplyMatrix(tMatrix *a, tMatrix *b, tMatrix *c)
{
	c->mx1 = a->mx1 * b->mx1;
	c->mx2 = a->mx2 * b->mx2;
	c->mx3 = a->mx3 * b->mx3;
	c->my1 = a->my1 * b->my1;
	c->my2 = a->my2 * b->my2;
	c->my3 = a->my3 * b->my3;
	c->mz1 = a->mz1 * b->mz1;
	c->mz2 = a->mz2 * b->mz2;
	c->mz3 = a->mz3 * b->mz3;
}

boolean isVisible(tVector *v)
{
	if (v == notVisible)
		return no;
	
	return yes;
}
	
void updateVectors(tVector *eyeVector, tVector *eyeLocation, tMatrix *eyeMatrix, tMatrix *M, u16bit vertices, tVector *from, tVector *too, tVector *fromN, tVector *tooN, tScalar x, tScalar y, tScalar z)
{
	u16bit i;
	u8bit c;
	tVector *v = from;
	tVector dv;
	tVector *ev = too;
    tVector *vn = fromN;
    tVector *evn = tooN;
    tMatrix EM;
    
    //multiplyMatrix(eyeMatrix, M, &EM);
    
	for (i=0; i<vertices; i++) {
		
		/* rotate to object */
		multiply(M, v, &dv);
		
		/* object to world space */
		dv.x += x;
		dv.y += y;
		dv.z += z;
		
		/* world to camera space */
		dv.x -= eyeLocation->x;
		dv.y -= eyeLocation->y;
		dv.z -= eyeLocation->z;
		
		/* test visible */		
		c = angle(eyeVector, &dv);
		if (c < 43) {
		
			/* rotate to eye */
			multiply(M, vn, evn);
			multiply(eyeMatrix, &dv, ev);
	        
			depthEmulate(ev);
			viewAdjust(ev);

			//if (onScreen(ev) == yes)
			//	drawPoint(ev, i+1);
		} else {
			evn = notVisible;
		}
		vn++;
		evn++;
		v++;
		ev++;
	}
}