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

boolean shut_down = no;
boolean space = no;

void key_action();

u8bit eX, eY, eZ, sX, sY, sZ;
tScalar eyeSpeed;

#define maxSpeed 500

void main()
{
	initSineCosine();
	initFaceHandler(4);
	init_anim();
	
	char buff[256];
	
    tMatrix eyeM;
    tMatrix M;
    
    tVector too[4];
    tVector tooN[4];
    
    tFace face[4];
    face[0].vertex[0] = 1;
    face[0].vertex[1] = 2;
    face[0].vertex[2] = 3;
    face[0].data = 1;
    face[1].vertex[0] = 0;
    face[1].vertex[1] = 2;
    face[1].vertex[2] = 3;
    face[1].data = 2;
    face[2].vertex[0] = 2;
    face[2].vertex[1] = 0;
    face[2].vertex[2] = 1;
    face[2].data = 3;
    face[3].vertex[0] = 3;
    face[3].vertex[1] = 0;
    face[3].vertex[2] = 1;
    face[3].data = 4;
    
    eyeSpeed = 0;
    eX = eY = eZ = sX = sY = sZ = 0;
    tVector fromN[4];
    tVector from[] = {0, 27, 0,
    				32, -27, 32,
    				-32, -27, 32,
    				0, -27, -32};
    				
	for (int i=0; i<4; i++) {
		fromN[i] = from[i];
		normalize(&fromN[i]);
	}
	
    tVector eyeN[] = {0, 0, 1};
    tVector t;
    tVector eyeL[] = {0, 0, -64};
    tVector lightN[] = {0, 0, 1};
    
    set_dac_register(1, 255, 255, 0);	// origin = yelo
    set_dac_register(2, 255, 0, 0);	// X-Axis = red
    set_dac_register(3, 0, 255, 0); // Y-Axis = green
    set_dac_register(4, 0, 0, 255); // Z-Axis = blue
    
    for (i=0; i<32; i++)
	    set_dac_register(5+i, 64, 196+i*2, 128); // gouraud
    
	while (shut_down == no)
	{
		if (space) wait_for_key(' ');
		if (kbhit()) key_action();

		computeRotMatrix(&eyeM, eX, eY, eZ);
		
		// make sure when rotating the eye to multiply the normal
		multiply(&eyeM, eyeN, &t);

		// move the eyeLocation
		eyeL->x += eyeSpeed * t.x;
		eyeL->y += eyeSpeed * t.y;
		eyeL->z += eyeSpeed * t.z;
		
		computeRotMatrix(&M, sX, sY, sZ);		
		updateVectors(&t, eyeL, &eyeM, &M, 4, from, too, fromN, tooN, 0, 0, 200);

		// register, sort and draw the faces
		for (i=0; i<4; i++)
			registerFace(&face[i], too, tooN);
			
		sortFaces();
		drawAllFaces(too, tooN, lightN);
		
		
		//drawGMFace(&face[2], too, tooN, lightN);
		
		sX++;
		sY++;
		//sZ++;
        
		// info
		itoa((int)eX, buff, 10);
		tprint_str("eyeX:", 5, 0, 350, 1);
		tprint_str(buff, 3, 60, 350, 2);

		itoa((int)eY, buff, 10);
		tprint_str("eyeY:", 5, 0, 330, 1);
		tprint_str(buff, 3, 60, 330, 2);

		itoa((int)eZ, buff, 10);
		tprint_str("eyeZ:", 5, 0, 310, 1);
		tprint_str(buff, 3, 60, 310, 2);

		itoa((int)eyeL->x, buff, 10);
		tprint_str("eyeLoX:", 7, 0, 290, 1);
		tprint_str(buff, 8, 60, 290, 2);
		itoa((int)eyeL->y, buff, 10);
		tprint_str("eyeLoY:", 7, 0, 270, 1);
		tprint_str(buff, 8, 60, 270, 2);
		itoa((int)eyeL->z, buff, 10);
		tprint_str("eyeLoZ:", 7, 0, 250, 1);
		tprint_str(buff, 8, 60, 250, 2);
		
		flip_page();
		
	}
/*    
    int j, r, g, b;
    
    for (j=0; j<256; j++) {
	    for (i=0; i<256; i++) {
	    	get_dac_register(i, &r, &g, &b);
	    	if (r > 0) r--;
	    	if (g > 0) g--;
	    	if (b > 0) b--;
	    	set_dac_register(i, r, g, b);
	    }
	    // delay
	    for (i=0; i<20000; i++) {
	    	r = r * r * g * b * g;
	    }
	    for (i=0; i<20000; i++) {
	    	r = r * r * g * b * g;
	    }
	    for (i=0; i<20000; i++) {
	    	r = r * r * g * b * g;
	    }
	    for (i=0; i<20000; i++) {
	    	r = r * r * g * b * g;
	    }
	}   
*/	
	kill_anim();
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
		case 'a':
			eyeSpeed = eyeSpeed + 1.0;
			if (eyeSpeed > maxSpeed) eyeSpeed = maxSpeed;
			break;
		case 'z':
			eyeSpeed = eyeSpeed - 1.0;
			if (eyeSpeed < 0) eyeSpeed = 0;
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