#include "face.h"
#include "anim_sys.h"
#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include "shape.h"

/*****************************
*****************************/
#define closestDistance 64

tScalar *faceZ;
tFace **faces;
tShape **facesS;
int maxCount;
int curCount;

tScalar dd;

/* drawing vars */
tMatrix B;
tScalar atx, btx, ctx, aty, bty, cty;
tScalar ad, ae, af, ag, ah, ai, det;
tScalar a, b, c;
tVector *max, *left, *right;
tVector *v[3];
tScalar lLen, rLen, ldx, rdx, lx, rx;
int ltx, rtx, y, len, x, tx, ty;
boolean wasLeft;
tScalar aftx[screen_width];
tScalar afty[screen_width];
tScalar ax[screen_width];
tScalar *zbf;
tScalar dist;
unsigned int offset, yoffset;
u8bit *scr;
tScalar s1, s2, s3;

inline void swap(tScalar *a, tScalar *b) {
	tScalar t = *a;
	*a = *b;
	*b = t;
}

inline void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

inline void swap(u8bit *a, u8bit *b) {
	u8bit t = *a;
	*a = *b;
	*b = t;
}

void initFaceHandler(int n)
{
	curCount = 0;
	maxCount = n;
	
	faceZ = new tScalar[maxCount];
	if (faceZ == NULL) {
		printf("falied to alloc mem for %d faceZ\n", maxCount);
		exit(0);
	}
	faces = (tFace**)calloc(maxCount, sizeof(tFace*));
	if (faces == NULL) {
		printf("falied to alloc mem for %d faces\n", maxCount);
		exit(0);
	}
	facesS = (tShape**)calloc(maxCount, sizeof(tShape*));
	if (facesS == NULL) {
		printf("falied to alloc mem for %d facesS\n", maxCount);
		exit(0);
	}
}

void killFaceHandler()
{
	printf("%d triangles in scene\n", maxCount);
	delete []faceZ;
	free(faces);
	free(facesS);
}

void addFace(tFace *face, void *shape)
{
	if (curCount >= maxCount)
		return;
	faces[curCount] = face;
	facesS[curCount] = (tShape*)shape;
	curCount++;
}

void sortFaces()
{
	tScalar tmp;
	tFace *tmp1;
	tShape *tmp2;
	int i, k, j;
	char found=0;

	k = curCount;
	while (k > 0) {
		k--;
		tmp = 0;
		for (i=0; i<3; i++) {
			j = faces[k]->vertex[i];
			tmp += facesS[k]->too[j].z;
		}
		/* 1 / tmp / 3 simplifies to 3 / tmp */
		if (tmp != 0)
			faceZ[k] = 3 / tmp;
		else
			faceZ[k] = 0;
	}

	for (k=1; k<curCount; k++)
	{
		tmp = faceZ[k];
		tmp1 = faces[k];
		tmp2 = facesS[k];
		i = k - 1;
		found = 0;
		while ((i >= 0) && (!found)) {
			if (tmp > faceZ[i]) {
				faceZ[i+1] = faceZ[i];
				faces[i+1] = faces[i];
				facesS[i+1] = facesS[i];
				i--;
			}
			else found=1;
		}
		faceZ[i+1] = tmp;
		faces[i+1] = tmp1;
		facesS[i+1] = tmp2;
	}
}

void drawAllFaces()
{	
	while (curCount > 0) {
		curCount--;
		drawGMFace(faces[curCount], facesS[curCount]->too, facesS[curCount]->texture);
	}
}


/**********************
gouraud Matrix method
**********************/
void drawGMFace(tFace *face, tVector vect[], u8bit texture[])
{	
	int i, j, k, maxx;
	tScalar minx, dby;
	u8bit col = (u8bit)face->data;
	int *t = &face->t[0];
		
	minx = 9999;
	maxx = -9999;
	/* get the three points */
	for (i=0; i<3; i++) {
		k = face->vertex[i];
		v[i] = &vect[k];
		if (v[i]->x < minx)
			minx = v[i]->x;
		if (v[i]->x > maxx)
			maxx = (int)v[i]->x;
	}

	/* find the top vertex (this is upside down!) */
	max = v[0];
	i = 0;
	if (v[1]->y < max->y) {
		max = v[1];
		i = 1;
	}
	if (v[2]->y < max->y) {
		max = v[2];
		i = 2;
	}

	/* compute the z! a, b, c vals */	
	ad = v[0]->x * v[1]->y;
	ae = v[0]->y * v[2]->x;
	af = v[1]->x * v[2]->y;
	ag = v[2]->x * v[1]->y;
	ah = v[2]->y * v[0]->x;
	ai = v[1]->x * v[0]->y;

	B[m11] = v[1]->y - v[2]->y;
	B[m12] = v[0]->y - v[2]->y;
	B[m13] = v[0]->y - v[1]->y;
	B[m21] = v[1]->x - v[2]->x;
	B[m22] = v[0]->x - v[2]->x;
	B[m23] = v[0]->x - v[1]->x;
	B[m31] = af - ag;
	B[m32] = ah - ae;
	B[m33] = ad - ai;
	
	a = B[m11] * v[0]->z - B[m12] * v[1]->z + B[m13] * v[2]->z;
	b = - B[m21] * v[0]->z + B[m22] * v[1]->z - B[m23] * v[2]->z;
	c = B[m31] * v[0]->z - B[m32] * v[1]->z + B[m33] * v[2]->z;

	atx = B[m11] * t[0] - B[m12] * t[2] + B[m13] * t[4];
	btx = - B[m21] * t[0] + B[m22] * t[2] - B[m23] * t[4];
	ctx = B[m31] * t[0] - B[m32] * t[2] + B[m33] * t[4];
	
	aty = B[m11] * t[1] - B[m12] * t[3] + B[m13] * t[5];
	bty = - B[m21] * t[1] + B[m22] * t[3] - B[m23] * t[5];
	cty = B[m31] * t[1] - B[m32] * t[3] + B[m33] * t[5];
    
	det = ad + ae + af - ag - ah - ai;
	if (det != 0)
		det = 1 / det;
	
	a *= det;
	b *= det;
	c *= det;
	
	atx *= det;
	btx *= det;
	ctx *= det;

	aty *= det;
	bty *= det;
	cty *= det;
    
    /* store the x iterations in an array */
    s1 = a * minx;
    s2 = atx * minx;
    s3 = aty * minx;
    x = (int)minx;
    while (x <= maxx) {
    	ax[x] = s1;
    	aftx[x] = s2;
    	afty[x] = s3;
    	x++;
    	s1 += a;
    	s2 += atx;
    	s3 += aty;
    }
    
	/* get the two adjacent vertexes */
	j = i - 1;
	if (j < 0) j = 2;
	k = i + 1;
	if (k > 2) k = 0;
	
	/* compute partial slopes */
	left = v[j];
	right = v[k];
	
	lLen = left->y - max->y;
	if ((int)lLen != 0)
		ldx = (left->x - max->x) / lLen;
	else
		ldx = left->x - max->x;
	rLen = right->y - max->y;
	if ((int)rLen != 0)
		rdx = (right->x - max->x) / rLen;
	else
		rdx = right->x - max->x;
	
	/* correct left right orientation */
	if (rdx < ldx) {
		swap(&j, &k);
		swap(&lLen, &rLen);		
		swap(&rdx, &ldx);		
		left = v[j];
		right = v[k];
	}
	
	/* pick short edge and begin draw */
	len = (int)rLen;
	wasLeft = no;
	if (lLen < rLen) {
		len = (int)lLen;
		wasLeft = yes;
	}
	
	lx = rx = max->x;
	y = (int)max->y;
	dby = max->y * b;
	s1 = max->y * btx;
	s2 = max->y * bty;
	yoffset = (y << 8) + (y << 6);
	
	while (len > 0) {
	
		if (y >= screen_height) return;
		
		ltx = (int)lx;
		rtx = (int)rx;
		
		/* clip lx, rx */
		if (ltx < 0)
			ltx = 0;
		if (rtx >= screen_width)
			rtx = screen_width - 1;
			
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0)) {
		    
		    offset = yoffset + ltx;
            scr = &out_buff[offset];
            zbf = &z_buff[offset];
			while (ltx <= rtx) {
				dist = ax[ltx] + dby + c;
				if (dist > *zbf) {
					*zbf = dist;
					tx = (int)(aftx[ltx] + s1 + ctx);
					tx = tx % 256;
					ty = (int)(afty[ltx] + s2 + cty);
					ty = ty % 256;
					tx += (ty << 7) + (ty << 6) + (ty << 3);
					*scr = texture[tx];
				}
				scr++;
				zbf++;
				ltx++;
			}
		}
			
		lx += ldx;
		rx += rdx;
		y++;
		len--;
		dby += b;
		s1 += btx;
		s2 += bty;
		yoffset += screen_width;
	}
	
	/* draw last line accounting for error buildup */
	if (y >= screen_height) return;
	if (wasLeft == yes) {
		lx = left->x;
	} else {
		rx = right->x;
	}
	ltx = (int)lx;
	rtx = (int)rx;

	if (y > max->y) {
					
		if (ltx < 0)
			ltx = 0;
		if (rtx >= screen_width)
			rtx = screen_width - 1;
					
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0)) {
		    
		    offset = yoffset + ltx;
            scr = &out_buff[offset];
            zbf = &z_buff[offset];
			while (ltx <= rtx) {
				dist = ax[ltx] + dby + c;
				if (dist > *zbf) {
					*zbf = dist;
					tx = (int)(aftx[ltx] + s1 + ctx);
					tx = tx % 256;
					ty = (int)(afty[ltx] + s2 + cty);
					ty = ty % 256;
					tx += (ty << 7) + (ty << 6) + (ty << 3);
					*scr = texture[tx];
				}
				scr++;
				zbf++;
				ltx++;
			}
		}		
		y++;
		dby += b;
		s1 += btx;
		s2 += bty;
		yoffset += screen_width;
	}
    
	/* short edge is done so get next and */
	if (wasLeft == yes) {
		lLen = rLen - lLen;
		if ((int)lLen == 0)
			ldx = right->x - left->x;			
		else
			ldx = (right->x - left->x) / lLen;
		len = (int)lLen;
	} else {
		rLen = lLen - rLen;
		if ((int)rLen == 0)
			rdx = left->x - right->x;
		else
			rdx = (left->x - right->x) / rLen;
		len = (int)rLen;
	}
	if (len == 0) {
		rx = right->x;
		lx = left->x;
	}
	
	/* finish last edge */
	while (len >= 0) {
	
		if (y >= screen_height) return;
		
		ltx = (int)lx;
		rtx = (int)rx;
		
		/* clip lx, rx */
		if (ltx < 0)
			ltx = 0;
		if (rtx >= screen_width)
			rtx = screen_width - 1;
			
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0)) {
		
		    offset = yoffset + ltx;
            scr = &out_buff[offset];
            zbf = &z_buff[offset];
			while (ltx <= rtx) {
				dist = ax[ltx] + dby + c;
				if (dist > *zbf) {
					*zbf = dist;
					tx = (int)(aftx[ltx] + s1 + ctx);
					tx = tx % 256;
					ty = (int)(afty[ltx] + s2 + cty);
					ty = ty % 256;
					tx += (ty << 7) + (ty << 6) + (ty << 3);
					*scr = texture[tx];
				}
				scr++;
				zbf++;
				ltx++;
			}
		}
			
		lx += ldx;
		rx += rdx;
		y++;
		len--;
		dby += b;
		s1 += btx;
		s2 += bty;
		yoffset += screen_width;
	}
}


/*****************************************************************************************
*****************************************************************************************/

		
/**********************
texture Matrix method
**********************/
void drawTFace(tFace *face, tVector vect[], u8bit texture[], int size)
{	
	int i, j, k;
	int minx, maxx;
	int *t = &face->t[0];
	
	minx = 9999;
	maxx = -9999;
	/* get the three points */
	for (i=0; i<3; i++) {
		k = face->vertex[i];
		v[i] = &vect[k];
		if (v[i]->x < minx)
			minx = (int)v[i]->x;
		if (v[i]->x > maxx)
			maxx = (int)v[i]->x;
	}

	/* find the top vertex (this is upside down!) */
	max = v[0];
	i = 0;
	if (v[1]->y < max->y) {
		max = v[1];
		i = 1;
	}
	if (v[2]->y < max->y) {
		max = v[2];
		i = 2;
	}

	/* compute the light a, b, c vals */	
	ad = v[0]->x * v[1]->y;
	ae = v[0]->y * v[2]->x;
	af = v[1]->x * v[2]->y;
	ag = v[2]->x * v[1]->y;
	ah = v[2]->y * v[0]->x;
	ai = v[1]->x * v[0]->y;
	
	B[m11] = v[1]->y - v[2]->y;
	B[m12] = v[0]->y - v[2]->y;
	B[m13] = v[0]->y - v[1]->y;
	B[m21] = v[1]->x - v[2]->x;
	B[m22] = v[0]->x - v[2]->x;
	B[m23] = v[0]->x - v[1]->x;
	B[m31] = af - ag;
	B[m32] = ah - ae;
	B[m33] = ad - ai;
	
	atx = B[m11] * t[0] - B[m12] * t[2] + B[m13] * t[4];
	btx = - B[m21] * t[0] + B[m22] * t[2] - B[m23] * t[4];
	ctx = B[m31] * t[0] - B[m32] * t[2] + B[m33] * t[4];
	
	aty = B[m11] * t[1] - B[m12] * t[3] + B[m13] * t[5];
	bty = - B[m21] * t[1] + B[m22] * t[3] - B[m23] * t[5];
	cty = B[m31] * t[1] - B[m32] * t[3] + B[m33] * t[5];

	det = ad + ae + af - ag - ah - ai;
	det = 1 / det;
	
	atx *= det;
	btx *= det;
	ctx *= det;

	aty *= det;
	bty *= det;
	cty *= det;
    
    /* store the x iterations in an array */
    x = minx;
    while (x <= maxx) {
    	aftx[x] = atx * x;
    	afty[x] = aty * x;
    	x++;
    }
    
	/* get the two adjacent vertexes */
	j = i - 1;
	if (j < 0) j = 2;
	k = i + 1;
	if (k > 2) k = 0;
	
	/* compute partial slopes */
	left = v[j];
	right = v[k];
	
	lLen = left->y - max->y;
	if ((int)lLen != 0)
		ldx = (left->x - max->x) / lLen;
	else
		ldx = left->x - max->x;
	rLen = right->y - max->y;
	if ((int)rLen != 0)
		rdx = (right->x - max->x) / rLen;
	else
		rdx = right->x - max->x;
	
	/* correct left right orientation */
	if (rdx < ldx) {
		swap(&j, &k);
		swap(&lLen, &rLen);		
		swap(&rdx, &ldx);		
		left = v[j];
		right = v[k];
	}
	
	/* pick short edge and begin draw */
	len = (int)rLen;
	wasLeft = no;
	if (lLen < rLen) {
		len = (int)lLen;
		wasLeft = yes;
	}
	
	lx = rx = max->x;
	y = (int)max->y;
	
	while (len > 0) {
	
		if (y >= screen_height) return;
		
		ltx = (int)lx;
		rtx = (int)rx;
		
		/* clip lx, rx */
		if (ltx < 0)
			ltx = 0;
		if (rtx >= screen_width)
			rtx = screen_width - 1;
			
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0)) {
		
			x = ltx;
            scr = (u8bit*)out_buff;
            scr += (y << 8) + (y << 6) + x;
            b = btx * y;
            c = bty * y;
			while (x <= rtx) {
				tx = (int)(aftx[x] + b + ctx);
				tx = tx % size;
				ty = (int)(afty[x] + c + cty);
				ty = ty % size;
				tx += (ty << 7) + (ty << 6) + (ty << 3);
				*scr = texture[tx];
				scr++;
				x++;
			}
		}
			
		lx += ldx;
		rx += rdx;
		y++;
		len--;
	}
	
	/* draw last line accounting for error buildup */
	if (y >= screen_height) return;
	if (wasLeft == yes) {
		lx = left->x;
		ltx = (int)lx;
	} else {
		rx = right->x;
		rtx = (int)rx;
	}

	if (y > max->y) {
					
		/* clip ltx, rtx */
		if (ltx < 0)
			ltx = 0;
		if (rtx >= screen_width)
			rtx = screen_width - 1;
					
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0)) {
		
			x = ltx;
            scr = (u8bit*)out_buff;
            scr += (y << 8) + (y << 6) + x;
            b = btx * y;
            c = bty * y;
			while (x <= rtx) {
				tx = (int)(aftx[x] + b + ctx);
				tx = tx % size;
				ty = (int)(afty[x] + c + cty);
				ty = ty % size;
				tx += (ty << 7) + (ty << 6) + (ty << 3);
				*scr = texture[tx];
				scr++;
				x++;
			}
		}
		y++;
	}
    
	/* short edge is done so get next and */
	if (wasLeft == yes) {
		lLen = rLen - lLen;
		if ((int)lLen == 0)
			ldx = right->x - left->x;
		else
			ldx = (right->x - left->x) / lLen;
		len = (int)lLen;
	} else {
		rLen = lLen - rLen;
		if ((int)rLen == 0)
			rdx = left->x - right->x;
		else
			rdx = (left->x - right->x) / rLen;
		len = (int)rLen;
	}
	
	/* finish last edge */
	while (len >= 0) {
	
		if (y >= screen_height) return;
		
		ltx = (int)lx;
		rtx = (int)rx;
		
		/* clip lx, rx */
		if (ltx < 0)
			ltx = 0;
		if (rtx >= screen_width)
			rtx = screen_width - 1;
			
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0)) {
		
			x = ltx;
            scr = (u8bit*)out_buff;
            scr += (y << 8) + (y << 6) + x;
            b = btx * y;
            c = bty * y;
			while (x <= rtx) {
				tx = (int)(aftx[x] + b + ctx);
				tx = tx % size;
				ty = (int)(afty[x] + c + cty);
				ty = ty % size;
				tx += (ty << 7) + (ty << 6) + (ty << 3);
				*scr = texture[tx];
				scr++;
				x++;
			}
		}
			
		lx += ldx;
		rx += rdx;
		y++;
		len--;
	}
}

/**********************************
	solid
**********************************/
	
void drawFace(tFace *face, tVector vect[], u8bit color)
{
	int i, j, k;

	/* get the three points */
	for (i=0; i<3; i++)
		v[i] = &vect[face->vertex[i]];
		
	/* find the top vertex */
	max = v[0];
	i = 0;
	if (v[1]->y < max->y) {
		max = v[1];
		i = 1;
	}
	if (v[2]->y < max->y) {
		max = v[2];
		i = 2;
	}

	/* get the two adjacent vertexes */
	j = i - 1;
	if (j < 0) j = 2;
	k = i + 1;
	if (k > 2) k = 0;
	
	/* compute partial slopes */
	left = v[j];
	right = v[k];
	
	ldx = rdx = 0;
	
	lLen = left->y - max->y;
	if ((int)lLen != 0)
		ldx = (left->x - max->x) / lLen;
	else
		ldx = left->x - max->x;
	rLen = right->y - max->y;
	if ((int)rLen != 0)
		rdx = (right->x - max->x) / rLen;
	else
		rdx = right->x - max->x;
	
	/* correct left right orientation */
	if (rdx < ldx) {
		swap(&j, &k);
		swap(&lLen, &rLen);		
		swap(&rdx, &ldx);		
		left = v[j];
		right = v[k];
	}
	
	/* pick short edge and begin draw */
	len = (int)rLen;
	wasLeft = no;
	if (lLen < rLen) {
		len = (int)lLen;
		wasLeft = yes;
	}
	
	lx = rx = max->x;
	y = (int)max->y;
	
	while (len > 0) {
	
		if (y >= screen_height) return;
		
		ltx = (int)lx;
		rtx = (int)rx;
		
		/* clip lx, rx */
		if (ltx < 0)
			ltx = 0;
		if (rtx >= screen_width)
			rtx = screen_width - 1;
			
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0)) {
			x = ltx;
            scr = (u8bit*)out_buff;
            scr += (y << 8) + (y << 6) + x;
			while (x <= rtx) {
				*scr = color;
				scr++;
				x++;
			}
		}
						
		lx += ldx;
		rx += rdx;
		y++;
		len--;
	}
	
	/* draw last line accounting for error buildup */
	if (y >= screen_height) return;
	if (wasLeft == yes) {
		lx = left->x;
		ltx = (int)lx;
	} else {
		rx = right->x;
		rtx = (int)rx;
	}

	if (y > max->y) {
					
		/* clip ltx, rtx */
		if (ltx < 0)
			ltx = 0;
		if (rtx >= screen_width)
			rtx = screen_width - 1;
					
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0)) {
			x = ltx;
            scr = (u8bit*)out_buff;
            scr += (y << 8) + (y << 6) + x;
			while (x <= rtx) {
				*scr = color;
				scr++;
				x++;
			}
		}		    
		y++;
	}
    
	/* short edge is done so get next and */
	if (wasLeft == yes) {
		lLen = rLen - lLen;
		if ((int)lLen != 0)
			ldx = (right->x - left->x) / lLen;
		else
			ldx = right->x - left->x;
		len = (int)lLen;
	} else {
		rLen = lLen - rLen;
		if ((int)rLen != 0)
			rdx = (left->x - right->x) / rLen;
		else
			rdx = left->x - right->x;
		len = (int)rLen;
	}
	
	/* finish last edge */
	while (len >= 0) {
	
		if (y >= screen_height) return;
		
		ltx = (int)lx;
		rtx = (int)rx;
		
		/* clip lx, rx */
		if (ltx < 0)
			ltx = 0;
		if (rtx >= screen_width)
			rtx = screen_width - 1;
			
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0)) {
			x = ltx;
            scr = (u8bit*)out_buff;
            scr += (y << 8) + (y << 6) + x;
			while (x <= rtx) {
				*scr = color;
				scr++;
				x++;
			}
		}
			
		lx += ldx;
		rx += rdx;
		y++;
		len--;
	}
}