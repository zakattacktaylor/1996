#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
#include <iostream.h>
extern "C" {
	#include "modex.h"
}
#include "types.h"
#include "anim_sys.h"
#include "vector.h"
#include "face.h"
#include "keyboard.h"
#include "ripper.h"

#define maxSpeed 500
#define vertices 4
#define faces 4

void key_action();
void output(tScalar n, char far *label, int len);
void initialize();
void destruct();

int i, k;
boolean shut_down = no;
boolean space = no;
u8bit eX, eY, eZ, sX, sY, sZ;
tMatrix eyeM;
tMatrix M;    
tVector too[vertices];
tVector hold[vertices];
tVector tooN[vertices];
tVector a, b, c;
tScalar lVals[vertices];
tScalar d;
tFace face[faces];
tVector fromN[vertices];
tVector from[] = {0, 27, 0, 
				32, -27, 32,
				-32, -27, 32,
				0, -27, -32};
tVector eyeL[] = {0, 0, -64};
tVector lightN[] = {0, 0, 1};
tVector lightPoint[] = {0, 0, -64};
u8bit texture[64*64];

void main()
{   
	if (rip_texture64(texture, "texture1.bmp") == no) {
		printf("rip failed\n");
		exit(0);
	}

	initialize();
			
	while (shut_down == no)
	{
		if (space) wait_for_key(' ');
		if (kbhit()) key_action();

		computeRotMatrix(&eyeM, eX, eY, eZ);
		computeRotMatrix(&M, sX, sY, sZ);		
        
		for (i=0; i<vertices; i++) {
	
	        /* transform points to world space */
			multiply(&M, &from[i], &hold[i]);			
			hold[i].x += 0;
			hold[i].y += 0;
			hold[i].z += 100;

			/* get a normal from the eye to each point */
			subtractVectors(&hold[i], eyeL, &tooN[i]);
		}
		
		/* determine face visibility */
		for (i=0; i<faces; i++) {
			k = face[i].vertex[0];
			normalize(&tooN[k]);
			multiply(&M, &face[i].n, &b);
			if (dotProduct(&b, &tooN[k]) <= 0)
				addFace(&face[i]);
				
			// could mark bit array for the faces vertexes needing normal transform
			// opt
		}
		
		for (i=0; i<vertices; i++) {
		    
		    a = hold[i];
		    
			/* world to eye space */
			a.x -= eyeL->x;
			a.y -= eyeL->y;
			a.z -= eyeL->z;

			/* rotate to eye */
			multiply(&M, &fromN[i], &tooN[i]);
			multiply(&eyeM, &a, &too[i]);
		        
			depthEmulate(&too[i]);
			viewAdjust(&too[i]);
		}
        
		/* compute face distances and sort */			
		sortFaces(too);

#define Specular 48400.0	// 220^2 !
		
		/* compute light values and draw */
		for (i=0; i<vertices; i++) {
			subtractVectors(&hold[i], lightPoint, &c);
			//d = c.x * c.x + c.y * c.y + c.z * c.z;			
			//output((int)d, "dist:", 5);
			normalize(&c);
			//d = Specular * lightValue(&tooN[i], &c) / d;
			//if (d > 1) d = 1;
			lVals[i] = (29 * lightValue(&tooN[i], &c)) + 7;
		}
		
		//draw_bitmap((char far*)&texture[0], 0, 0, 64, 64);
			
		drawAllFaces(too, lVals, texture);
		
		sX++;
		//sY++;
		//sZ++;
		
		flip_page();
		
	}
	
	destruct();
}

void output(tScalar n, char far *label, int len)
{
	char buff[10];
	int y = next_output_y();
		
	itoa((int)n, buff, 10);
	tprint_str(label, len, 0, y, 1);
	tprint_str(buff, 10, 10*len, y, 2);
}

void initialize()
{
	initSineCosine();
	initFaceHandler(faces);
	init_anim();
    
    face[0].vertex[0] = 3;
    face[0].vertex[1] = 0;
    face[0].vertex[2] = 2;
    face[0].data = 1;
    face[0].t[0] = 32;
    face[0].t[1] = 0;
    face[0].t[2] = 63;
    face[0].t[3] = 32;
    face[0].t[4] = 0;
    face[0].t[5] = 32;

    face[1].vertex[0] = 3;
    face[1].vertex[1] = 1;
    face[1].vertex[2] = 0;
    face[1].data = 2;
    face[1].t[0] = 32;
    face[1].t[1] = 0;
    face[1].t[2] = 63;
    face[1].t[3] = 32;
    face[1].t[4] = 0;
    face[1].t[5] = 32;

    face[2].vertex[0] = 0;
    face[2].vertex[1] = 1;
    face[2].vertex[2] = 2;
    face[2].data = 3;
    face[2].t[0] = 32;
    face[2].t[1] = 0;
    face[2].t[2] = 63;
    face[2].t[3] = 32;
    face[2].t[4] = 0;
    face[2].t[5] = 32;

    face[3].vertex[0] = 3;
    face[3].vertex[1] = 1;
    face[3].vertex[2] = 2;
    face[3].data = 4;
    face[3].t[0] = 32;
    face[3].t[1] = 0;
    face[3].t[2] = 63;
    face[3].t[3] = 32;
    face[3].t[4] = 0;
    face[3].t[5] = 32;
    
    eX = eY = eZ = sX = sY = sZ = 0;
    
    /* compute vertex normals */				
	for (i=0; i<vertices; i++) {
		fromN[i] = from[i];
		normalize(&fromN[i]);
	}
	
	/* compute face normals */
	for (i=0; i<faces; i++) {
		subtractVectors(&from[face[i].vertex[0]], &from[face[i].vertex[1]], &a);
		subtractVectors(&from[face[i].vertex[2]], &from[face[i].vertex[1]], &b);        
        crossProduct(&a, &b, &c);
        normalize(&c);
        face[i].n = c;
	}
    
    set_dac_register(1, 255, 255, 0);	// origin = yelo
    set_dac_register(2, 255, 0, 0);	// X-Axis = red
    set_dac_register(3, 0, 255, 0); // Y-Axis = green
    set_dac_register(4, 0, 0, 255); // Z-Axis = blue
    
    for (i=0; i<32; i++)
	    set_dac_register(5+i, 64, 196+i*2, 128); // gouraud
}
 
void destruct()
{
	kill_anim();
	printf("by zak :: christmas 96-97\n");
}

void key_action()
{
	u8bit key;

	key = getch();
	if (!key)
		key = getch();
		
	switch (key)
	{
		case 27:
			shut_down = yes;
			break;
		case 72:
			eX++;
			break;
		case 80:
			eX--;
			break;
		case 77:
			eY++;
			break;
		case 75:
			eY--;
			break;
		case 's':
			if (space == yes)
				space = no;
			else
				space = yes;
			break;
		default:
			break;
	}
}