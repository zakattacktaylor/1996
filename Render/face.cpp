#include "face.h"
#include "anim_sys.h"
#include "types.h"
extern "C" {
	#include "modex.h"
}
#include <stdlib.h>
#include <stdio.h>

/*****************************
*****************************/
#define closestDistance 64

tScalar *faceZ;
tFace **faces;
int maxCount;
int curCount;


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
}

void addFace(tFace *face)
{
	faces[curCount] = face;
	curCount++;
}

void sortFaces(tVector v[])
{
	tScalar tmp;
	tFace *tmp1;
	int i, k, j;
	char found=0;

	k = curCount;
	while (k > 0) {
		k--;
		tmp = 0;
		for (i=0; i<3; i++) {
			j = faces[k]->vertex[i];
			tmp += v[j].z;
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
		i = k - 1;
		found = 0;
		while ((i >= 0) && (!found)) {
			if (tmp > faceZ[i]) {
				faceZ[i+1] = faceZ[i];
				faces[i+1] = faces[i];
				i--;
			}
			else found=1;
		}
		faceZ[i+1] = tmp;
		faces[i+1] = tmp1;
	}
}

void drawAllFaces(tVector v[], tScalar lVals[], u8bit texture[])
{
	while (curCount > 0) {
		curCount--;
		drawTFace(faces[curCount], v, texture);
		//drawGMFace(faces[curCount], v, lVals);
		//drawGouraudFace(faces[curCount], v, vn, light);
		//drawFace(faces[curCount], v, (u8bit)faces[curCount]->data);
	}
}
		
/**********************
texture Matrix method
**********************/
void drawTFace(tFace *face, tVector vect[], u8bit texture[])
{
	int i, j, k;
	tVector *max, *left, *right;
	tVector *v[3];
	tScalar lLen, rLen, ldx, rdx, lx, rx;
	int ltx, rtx, y, len, x, tx, ty;
	boolean wasLeft;
	
	/* get the three points */
	for (i=0; i<3; i++) {
		k = face->vertex[i];
		v[i] = &vect[k];
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
	tScalar atx, btx, ctx, aty, bty, cty;
	tScalar ad, ae, af, ag, ah, ai, det;
	
	ad = v[0]->x * v[1]->y;
	ae = v[0]->y * v[2]->x;
	af = v[1]->x * v[2]->y;
	ag = v[2]->x * v[1]->y;
	ah = v[2]->y * v[0]->x;
	ai = v[1]->x * v[0]->y;
	
	atx = (v[1]->y - v[2]->y) * face->t[0] - (v[0]->y - v[2]->y) * face->t[2] + (v[0]->y - v[1]->y) * face->t[4];
	btx = - (v[1]->x - v[2]->x) * face->t[0] + (v[0]->x - v[2]->x) * face->t[2] - (v[0]->x - v[1]->x) * face->t[4];
	ctx = (af - ag) * face->t[0] - (ah - ae) * face->t[2] + (ad - ai) * face->t[4];
	
	aty = (v[1]->y - v[2]->y) * face->t[1] - (v[0]->y - v[2]->y) * face->t[3] + (v[0]->y - v[1]->y) * face->t[5];
	bty = - (v[1]->x - v[2]->x) * face->t[1] + (v[0]->x - v[2]->x) * face->t[3] - (v[0]->x - v[1]->x) * face->t[5];
	cty = (af - ag) * face->t[1] - (ah - ae) * face->t[3] + (ad - ai) * face->t[5];

	det = ad + ae + af - ag - ah - ai;
	det = 1 / det;
	
	atx *= det;
	btx *= det;
	ctx *= det;

	aty *= det;
	bty *= det;
	cty *= det;

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
		ldx = 0;
	rLen = right->y - max->y;
	if ((int)rLen != 0)
		rdx = (right->x - max->x) / rLen;
	else
		rdx = 0;
	
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
/*			ad = c;
			while (x < ltx) {
				x++;
				ad += a;
			} */
			while (x <= rtx) {
				tx = (int)(atx*x + btx*y + ctx);
				if (tx < 0) tx = -tx;
				tx = tx % 64;
				ty = (int)(aty*x + bty*y + cty);
				if (ty < 0) ty = -ty;
				ty = ty % 64;
				tx += ty * 64;
				set_point(x, y, texture[tx]);
				x++;
				//ad += a;
			}
		}
			
		lx += ldx;
		rx += rdx;
		y++;
//		ad = b*y + c;
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
/*			ad = c;
			while (x < ltx) {
				x++;
				ad += a;
			}*/
			while (x <= rtx) {
				tx = (int)(atx*x + btx*y + ctx);
				if (tx < 0) tx = -tx;
				tx = tx % 64;
				ty = (int)(aty*x + bty*y + cty);
				if (ty < 0) ty = -ty;
				ty = ty % 64;
				tx += ty * 64;
				set_point(x, y, texture[tx]);
				x++;
				//ad += a;
			}
		}
		
		//ad = b*y + c;    
		y++;
	}
    
	/* short edge is done so get next and */
	if (wasLeft == yes) {
		lLen = rLen - lLen;
		if ((int)lLen == 0)
			ldx = 0;
		else
			ldx = (right->x - left->x) / lLen;
		len = (int)lLen;
	} else {
		rLen = lLen - rLen;
		if ((int)rLen == 0)
			rdx = 0;
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
/*			ad = c;
			while (x < ltx) {
				x++;
				ad += a;
			}*/
			while (x <= rtx) {
				tx = (int)(atx*x + btx*y + ctx);
				if (tx < 0) tx = -tx;
				tx = tx % 64;
				ty = (int)(aty*x + bty*y + cty);
				if (ty < 0) ty = -ty;
				ty = ty % 64;
				tx += ty * 64;
				set_point(x, y, texture[tx]);
				x++;
				//ad += a;
			}
		}
			
		lx += ldx;
		rx += rdx;
		y++;
		//ad = b*y + c;
		len--;
	}
}

/**********************
gouraud Matrix method
**********************/
void drawGMFace(tFace *face, tVector vect[], tScalar lVals[])
{
	int i, j, k;
	tVector *max, *left, *right;
	tVector *v[3];
	tScalar lLen, rLen, ldx, rdx, lx, rx;
	int ltx, rtx, y, len, x, ol;
	boolean wasLeft;
	tScalar lval[3];
	
	/* get the three points */
	for (i=0; i<3; i++) {
		k = face->vertex[i];
		v[i] = &vect[k];
		lval[i] = lVals[k];
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
	tScalar a, b, c, ad, ae, af, ag, ah, ai, det;
	
	ad = v[0]->x * v[1]->y;
	ae = v[0]->y * v[2]->x;
	af = v[1]->x * v[2]->y;
	ag = v[2]->x * v[1]->y;
	ah = v[2]->y * v[0]->x;
	ai = v[1]->x * v[0]->y;
	
	a = (v[1]->y - v[2]->y) * lval[0] - (v[0]->y - v[2]->y) * lval[1] + (v[0]->y - v[1]->y) * lval[2];
	b = - (v[1]->x - v[2]->x) * lval[0] + (v[0]->x - v[2]->x) * lval[1] - (v[0]->x - v[1]->x) * lval[2];
	c = (af - ag) * lval[0] - (ah - ae) * lval[1] + (ad - ai) * lval[2];

	det = ad + ae + af - ag - ah - ai;
	det = 1 / det;
	
	a *= det;
	b *= det;
	c *= det;
/*    
	ad = maxx;
	if ((int)ad != 0)
		a = a / ad;
	ad = miny;
	if ((int)ad != 0)
		b = b / ad;
*/
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
		ldx = 0;
	rLen = right->y - max->y;
	if ((int)rLen != 0)
		rdx = (right->x - max->x) / rLen;
	else
		rdx = 0;
	
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
	ol = len;
	
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
/*			ad = c;
			while (x < ltx) {
				x++;
				ad += a;
			} */
			while (x <= rtx) {
				set_point(x, y, (u8bit)(a*x + b*y + c));
				x++;
				//ad += a;
			}
		}
			
		lx += ldx;
		rx += rdx;
		y++;
//		ad = b*y + c;
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
/*			ad = c;
			while (x < ltx) {
				x++;
				ad += a;
			}*/
			while (x <= rtx) {
				set_point(x, y, (u8bit)(a*x + b*y + c));
				x++;
				//ad += a;
			}
		}
		
		//ad = b*y + c;    
		y++;
	}
    
	/* short edge is done so get next and */
	if (wasLeft == yes) {
		lLen = rLen - lLen;
		if ((int)lLen == 0)
			ldx = 0;
		else
			ldx = (right->x - left->x) / lLen;
		len = (int)lLen;
	} else {
		rLen = lLen - rLen;
		if ((int)rLen == 0)
			rdx = 0;
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
/*			ad = c;
			while (x < ltx) {
				x++;
				ad += a;
			}*/
			while (x <= rtx) {
				set_point(x, y, (u8bit)(a*x + b*y + c));
				x++;
				//ad += a;
			}
		}
			
		lx += ldx;
		rx += rdx;
		y++;
		//ad = b*y + c;
		len--;
	}
}
	
void drawFace(tFace *face, tVector vect[], u8bit color)
{
	int i, j, k;
	tVector *max, *left, *right;
	tVector *v[3];
	tScalar lLen, rLen, ldx, rdx, lx, rx;
	int ltx, rtx, y, len;
	boolean wasLeft;
	
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
	rLen = right->y - max->y;
	if ((int)rLen != 0)
		rdx = (right->x - max->x) / rLen;
	
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
			
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0))
			fill_block(ltx, y, rtx, y, color);
			
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
					
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0))
			fill_block(ltx, y, rtx, y, color);
		    
		y++;
	}
    
	/* short edge is done so get next and */
	if (wasLeft == yes) {
		lLen = rLen - lLen;
		if ((int)lLen == 0) return;
		ldx = (right->x - left->x) / lLen;
		len = (int)lLen;
	} else {
		rLen = lLen - rLen;
		if ((int)rLen == 0) return;
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
			
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0))
			fill_block(ltx, y, rtx, y, color);
			
		lx += ldx;
		rx += rdx;
		y++;
		len--;
	}
}

/*****************
	gouraud
*****************/

void drawGouraudFace(tFace *face, tVector vect[], tVector vectN[], tVector *light)
{
	int i, j, k;
	tVector *max, *left, *right;
	tVector *v[3];
	tScalar lLen, rLen, ldx, rdx, lx, rx;
	int ltx, rtx, y, len;
	boolean wasLeft;
	u8bit lval[3];
	tScalar ldl, rdl, dl, li;
	int x;
	tScalar maxLi, lLi, rLi;

	/* get the three points */
	for (i=0; i<3; i++) {
		v[i] = &vect[face->vertex[i]];
		lval[i] = (u8bit)(31 * lightValue(&vectN[face->vertex[i]], light)) + 5;
	}
			
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
	maxLi = lval[i];

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
	rLen = right->y - max->y;
	if ((int)rLen != 0)
		rdx = (right->x - max->x) / rLen;
	
	/* compute the gouraud slopes */
	lLi = lval[j];
	rLi = lval[k];
	
	if ((int)lLen != 0)
		ldl = (lLi - maxLi) / lLen;
	if ((int)rLen != 0)
		rdl = (rLi - maxLi) / rLen;
		
	/* correct left right orientation */
	if (rdx < ldx) {
		swap(&j, &k);
		swap(&lLen, &rLen);		
		swap(&rdx, &ldx);
		swap(&ldl, &rdl);
		swap(&rLi, &lLi);
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
	lLi = rLi = maxLi;
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
		
			/* blend across */
			i = rtx - ltx;
			if ((int)i != 0) {
				
				dl = (rLi - lLi) / i;
				li = lLi;
				x = ltx;
				while (x < rtx) {
					set_point(x, y, (u8bit)li);
					li += dl;
					x++;
				}

				/* account for error */
				if (x > ltx) {
					li = rLi;
					set_point(x, y, (u8bit)li);
				}					
			}
		}
				
		lx += ldx;
		rx += rdx;
		lLi += ldl;
		rLi += rdl;
		y++;
		len--;
	}
	
	/* draw last line accounting for error buildup */
	if (y >= screen_height) return;
	if (wasLeft == yes) {
		lx = left->x;
		ltx = (int)lx;
		lLi = lval[j];
	} else {
		rx = right->x;
		rtx = (int)rx;
		rLi = lval[k];
	}

	if (y > max->y) {
					
		/* clip ltx, rtx */
		if (ltx < 0)
			ltx = 0;
		if (rtx >= screen_width)
			rtx = screen_width - 1;
					
		if ((ltx < screen_width) && (rtx > 0) && (y >= 0)) {
		
			/* blend across */
			i = rtx - ltx;
			if ((int)i != 0) {
				
				dl = (rLi - lLi) / i;
				li = lLi;
				x = ltx;
				while (x < rtx) {
					set_point(x, y, (u8bit)li);
					li += dl;
					x++;
				}

				/* account for error */
				if (x > ltx) {
					li = rLi;
					set_point(x, y, (u8bit)li);
				}					
			}
		}
		    
		y++;
	}
    
	/* short edge is done so get next and */
	if (wasLeft == yes) {
		lLen = rLen - lLen;
		if ((int)lLen == 0) return;
		ldx = (right->x - left->x) / lLen;
		len = (int)lLen;
		ldl = (lval[k] - lval[j]) / lLen;
	} else {
		rLen = lLen - rLen;
		if ((int)rLen == 0) return;
		rdx = (left->x - right->x) / rLen;
		len = (int)rLen;
		rdl = (lval[j] - lval[k]) / rLen;
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
		
			/* blend across */
			i = rtx - ltx;
			if ((int)i != 0) {
				
				dl = (rLi - lLi) / i;
				li = lLi;
				x = ltx;
				while (x < rtx) {
					set_point(x, y, (u8bit)li);
					li += dl;
					x++;
				}
				
				/* account for error */
				if (x > ltx) {
					li = rLi;
					set_point(x, y, (u8bit)li);
				}					
			}
		}
			
		lx += ldx;
		rx += rdx;
		lLi += ldl;
		rLi += rdl;
		y++;
		len--;
	}
}