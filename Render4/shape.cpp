#include "shape.h"
#include "face.h"
#include "vector.h"
#include "types.h"
#include "ripper.h"
#include "anim_sys.h"
#include <stdio.h>
#include <stdlib.h>

#define _xbound 50
#define _ybound 40

void bounce_shape(tShape *s)
{
	s->loc.x += s->dx;
	if (s->loc.x > _xbound) {
		s->dx = -s->dx;
		s->loc.x += s->dx;
	}
	if (s->loc.x < -_xbound) {
		s->dx = -s->dx;
		s->loc.x += s->dx;
	}
	
	s->loc.y += s->dy;
	if (s->loc.y > _ybound) {
		s->dy = -s->dy;
		s->loc.y += s->dy;
	}
	if (s->loc.y < -_ybound) {
		s->dy = -s->dy;
		s->loc.y += s->dy;
	}
}
	
tShape *new_shape(int vs, int fs)
{
	tShape *s;
	
	s = new tShape;
	if (!s) {
		printf("no mem for shape\n");
		exit(0);
	}	
	s->from = new tVector[vs];
	if (!s->from) {
		printf("no mem for shape\n");
		exit(0);
	}
	s->too = new tVector[vs];
	if (!s->too) {
		printf("no mem for shape\n");
		exit(0);
	}
	s->vertices = vs;	
	s->xan = s->yan = s->zan = 0;
	
	if (fs) {
		s->face = new tFace[fs];
		if (!s->face) {
			printf("no mem for shape\n");
			exit(0);
		}
	}			
	s->faces = fs;
	
	return s;
}

tShape *new_sphere(tScalar radius, int res, u8bit c1, u8bit c2)
{
	tScalar xan, yan, zan;
	tMatrix M;	
	tVector u, v, w, x, n;
	tScalar dan;
	tShape *s;
	int num_rings = (res / 2) - 1;
	int vertices = (res * num_rings) + 2;
	int faces = 2 * res * num_rings;
	int i, j, k;
	
	s = new_shape(vertices, faces);
	if (!s)
		return NULL;
	
	s->texture_size = 200;
	s->texture = new u8bit[40000];
	if (!s->texture) {
		printf("no mem for texture\n");
		exit(0);
	}	
	if (!rip_texture(s->texture, "swirl.bmp", s->texture_size)) {
		printf("rip failed\n");
		exit(0);
	}

	v.x = 0;
	v.y = radius;
	v.z = 0;
	i = 0;
	s->from[i] = v;
	i++;
	
	xan = yan = zan = 0;
	dan = 256.0 / res;
	
	for (j=0; j<num_rings; j++) {
		u = v;
		zan += dan;
		computeRotMatrix(M, 0, 0, zan);
		multiply(M, &u, &w);
		yan = 0;
		u = w;
		for (k=0; k<res; k++) {
			s->from[i] = u;
			i++;
			
			u = w;
			yan += dan;
			computeRotMatrix(M, 0, yan, 0);
			multiply(M, &u, &x);
			u = x;
		}
	}
	w.x = 0;
	w.y = -radius;
	w.z = 0;
	s->from[i] = w;
	
	/* faces */
	i = 0;
	u8bit color = c1;
	int offset = 0;
	tScalar dtx = (tScalar)200 / res;
	tScalar tx, ty;
	
	tx = 0;
	ty = 0;	
	for (k=0; k<res; k++) {
		s->face[i].vertex[0] = 0;
		s->face[i].vertex[1] = k + 1;
		s->face[i].vertex[2] = ((k + 1) % res) + 1;
		s->face[i].data = color;
		if (color == c2)
			color = c1;
		else
			color = c2;
		i++;
	   	s->face[i].t[0] = tx + (dtx / 2);
	    s->face[i].t[1] = ty;
	    s->face[i].t[2] = tx;
	    s->face[i].t[3] = ty + dtx;
	    s->face[i].t[4] = tx + dtx;
	    s->face[i].t[5] = ty + dtx;
	    
	    tx += dtx;
	}
	for (j=0; j<num_rings-1; j++) {
		ty += dtx;
		tx = 0;
		for (k=0; k<res; k++) {
			s->face[i].vertex[0] = k + (j * res) + 1;
			s->face[i].vertex[1] = ((k + 1) % res) + (j * res) + 1;
			s->face[i].vertex[2] = ((k + 1) % res) + ((j + 1) * res) + 1;
			s->face[i].data = c1;
		   	s->face[i].t[0] = tx;
		    s->face[i].t[1] = ty;
		    s->face[i].t[2] = tx + dtx;
		    s->face[i].t[3] = ty;
		    s->face[i].t[4] = tx + dtx;
		    s->face[i].t[5] = ty + dtx;
			i++;
			s->face[i].vertex[0] = k + (j * res) + 1;
			s->face[i].vertex[1] = ((k + 1) % res) + ((j + 1) * res) + 1;
			s->face[i].vertex[2] = k + ((j + 1) * res) + 1;
			s->face[i].data = c2;
		   	s->face[i].t[0] = tx;
		    s->face[i].t[1] = ty;
		    s->face[i].t[2] = tx + dtx;
		    s->face[i].t[3] = ty + dtx;
		    s->face[i].t[4] = tx;
		    s->face[i].t[5] = ty + dtx;
			i++;
			
			tx += dtx;
		}
	}
	j = res * (num_rings - 1);
	ty += dtx;
	tx = 0;
	for (k=0; k<res; k++) {
		s->face[i].vertex[0] = k + j + 1;
		s->face[i].vertex[1] = ((k + 1) % res) + j + 1;
		s->face[i].vertex[2] = j + res + 1;
		s->face[i].data = color;
		if (color == c2)
			color = c1;
		else
			color = c2;
	   	s->face[i].t[0] = tx;
	    s->face[i].t[1] = ty;
	    s->face[i].t[2] = tx + dtx;
	    s->face[i].t[3] = ty;
	    s->face[i].t[4] = tx + (dtx / 2);
	    s->face[i].t[5] = ty + dtx;
		i++;
	}		

	/* compute face normals */
	for (i=0; i<s->faces; i++) {
		k = s->face[i].vertex[1];
		subtractVectors(&s->from[s->face[i].vertex[0]], &s->from[k], &u);
		subtractVectors(&s->from[s->face[i].vertex[2]], &s->from[k], &v);
        crossProduct(&u, &v, &w);
        normalize(&w);
        /* pick a vertex */
        /* get a normal from the center to this vector */
        /* compare to the normal of this face and see if it is close */
        /* if it is within 0-1 leave it else flip it */
        u = s->from[k];
        normalize(&u);
        if (dotProduct(&u, &w) < 0) {
        	w.x = -w.x;
        	w.y = -w.y;
        	w.z = -w.z;
        }
        s->face[i].n = w;
	}
	
	return s;
}	
			
void update_shape(tShape *s, tMatrix E, tVector *eloc, tVector *lightPoint)
{
	int i, k;
	tVector a, b, c;
	 
	computeRotMatrix(s->M, s->xan, s->yan, s->zan);		
        
	for (i=0; i<s->vertices; i++) {
	
        /* transform points to world space */
		multiply(s->M, &s->from[i], &s->too[i]);			
		s->too[i].x += s->loc.x;
		s->too[i].y += s->loc.y;
		s->too[i].z += s->loc.z;
	}
		
	/* determine face visibility */
	for (i=0; i<s->faces; i++) {
		k = s->face[i].vertex[0];
		
		// get a normal from the eye to a vertex
		subtractVectors(&s->too[k], eloc, &c);
		normalize(&c);
		multiply(s->M, &s->face[i].n, &b);
		if (dotProduct(&b, &c) < 0)
			addFace(&s->face[i], (void*)s);
	}
		
	for (i=0; i<s->vertices; i++) {
		    
	    a = s->too[i];
		    
		/* world to eye space */
		a.x -= eloc->x;
		a.y -= eloc->y;
		a.z -= eloc->z;

		/* rotate to eye */
		multiply(E, &a, &s->too[i]);
		        
		depthEmulate(&s->too[i]);
		viewAdjust(&s->too[i]);
		
		if (s->too[i].z != 0)
			s->too[i].z = 1 / s->too[i].z;
			
		//if (onScreen(&s->too[i]))
		//	drawPoint(&s->too[i], 200);
	}
}

tShape *new_pyramid()
{
	tVector a, b, c;
	int i;
	tShape *s = new_shape(4, 4);
	
	s->texture_size = 200;
	
	s->texture = (u8bit*)malloc(40000);
	if (!s->texture) {
		printf("no mem for pyramid texture\n");
		exit(0);
	}
	if (!rip_texture(s->texture, "texture3.bmp", s->texture_size)) {
		printf("error\n");
		exit(0);
	}
	
	s->from[0].x = 0;
	s->from[0].y = 27;
	s->from[0].z = 0;
	s->from[1].x = 32;
	s->from[1].y = -27;
	s->from[1].z = 32;
	s->from[2].x = -32;
	s->from[2].y = -27;
	s->from[2].z = 32;
	s->from[3].x = 0;
	s->from[3].y = -27;
	s->from[3].z = -32;

    s->face[0].vertex[0] = 3;
    s->face[0].vertex[1] = 0;
    s->face[0].vertex[2] = 2;
    s->face[0].data = 1;
    s->face[0].t[0] = s->texture_size / 2;
    s->face[0].t[1] = 0;
    s->face[0].t[2] = s->texture_size - 1;
    s->face[0].t[3] = s->texture_size / 2;
    s->face[0].t[4] = 0;
    s->face[0].t[5] = s->texture_size / 2;

    s->face[1].vertex[0] = 3;
    s->face[1].vertex[1] = 1;
    s->face[1].vertex[2] = 0;
    s->face[1].data = 2;
    s->face[1].t[0] = s->texture_size / 2;
    s->face[1].t[1] = 0;
    s->face[1].t[2] = s->texture_size - 1;
    s->face[1].t[3] = s->texture_size / 2;
    s->face[1].t[4] = 0;
    s->face[1].t[5] = s->texture_size / 2;

    s->face[2].vertex[0] = 0;
    s->face[2].vertex[1] = 1;
    s->face[2].vertex[2] = 2;
    s->face[2].data = 3;
    s->face[2].t[0] = s->texture_size / 2;
    s->face[2].t[1] = 0;
    s->face[2].t[2] = s->texture_size - 1;
    s->face[2].t[3] = s->texture_size / 2;
    s->face[2].t[4] = 0;
    s->face[2].t[5] = s->texture_size / 2;

    s->face[3].vertex[0] = 3;
    s->face[3].vertex[1] = 1;
    s->face[3].vertex[2] = 2;
    s->face[3].data = 4;
    s->face[3].t[0] = s->texture_size / 2;
    s->face[3].t[1] = 0;
    s->face[3].t[2] = s->texture_size - 1;
    s->face[3].t[3] = s->texture_size / 2;
    s->face[3].t[4] = 0;
    s->face[3].t[5] = s->texture_size / 2;
	
	/* compute face normals */
	for (i=0; i<s->faces; i++) {
		subtractVectors(&s->from[s->face[i].vertex[0]], &s->from[s->face[i].vertex[1]], &a);
		subtractVectors(&s->from[s->face[i].vertex[2]], &s->from[s->face[i].vertex[1]], &b);        
        crossProduct(&a, &b, &c);
        normalize(&c);
        s->face[i].n = c;
	}
	
	return s;
}