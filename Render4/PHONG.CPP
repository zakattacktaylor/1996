#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
#include <iostream.h>
#include "types.h"
#include "anim_sys.h"
#include "vector.h"
#include "face.h"
#include "shape.h"
#include "ripper.h"

void key_action();
void wait_for_key(int ch);
void initialize();
void destruct();

boolean shut_down = no;
boolean space = no;
u8bit eX, eY, eZ;
tMatrix eyeM;
tVector eyeL[] = {0, 0, -64};
tVector lightPoint[] = {256, 256, -256};
tVector light;
u8bit lX, lY, lZ;

tShape *pyr;
tShape *sph;

void main()
{
	int x, y;
	
	initialize();
			
	while (shut_down == no)
	{
		if (space) wait_for_key(' ');
		if (kbhit()) key_action();
		
		// use for light (temp)
		computeRotMatrix(eyeM, lX, lY, lZ);
		multiply(eyeM, lightPoint, &light);

		computeRotMatrix(eyeM, eX, eY, eZ);
		
		update_shape(pyr, eyeM, eyeL, lightPoint);
		update_shape(sph, eyeM, eyeL, &light);
		
		//sortFaces();
		drawAllFaces();

/*		
		for (x=0; x<256; x++) {
			out_buff[(20*screen_width)+x] = (u8bit)x;
			out_buff[(21*screen_width)+x] = (u8bit)x;
			out_buff[(22*screen_width)+x] = (u8bit)x;
			out_buff[(23*screen_width)+x] = (u8bit)x;
		}
		
		for (x=0; x<200; x++) {
			for (y=0; y<200; y++) {
				out_buff[(y*screen_width)+x] = sph->texture[(y*200)+x];
			}
		}
*/			
		pyr->xan--;
		pyr->yan++;
		pyr->zan--;
		sph->xan++;
		sph->yan++;
		sph->zan++;
		
		lX--;
		lY--;
		lZ += 2;
		
		/* bounce them around a bit - (just x,y) */
		bounce_shape(sph);
		bounce_shape(pyr);
		
		flip_page();	
	}	
	destruct();
}

void initialize()
{
	int i;
	
	initSineCosine();
    eX = eY = eZ = 0;
    lX = lY = lZ = 0;

	sph = new_sphere(32, 12, 1, 2);
	if (!sph) {
		printf("no mem for sphere\n");
		exit(0);
	}
	sph->loc.x = -16;
	sph->loc.y = 0;
	sph->loc.z = 120;

	pyr = new_sphere(32, 12, 3, 4);
	if (!pyr) {
		printf("no mem for sphere2\n");
		exit(0);
	}
	pyr->loc.x = 16;
	pyr->loc.y = 8;
	pyr->loc.z = 110;

	sph->dx = -2;
	sph->dy = 3;
	pyr->dx = 2;
	pyr->dy = -1;

	initFaceHandler(sph->faces + pyr->faces);	
	init_anim();
    
    tScalar dx = 8.0 / 128.0;
    tScalar x = 0;

	/* ripper can load palettes !! */
	if (!rip_palette("palette.act")) {
		kill_anim();
		printf("failed to rip pal\n");
		exit(0);
	}
	
/*
    set_dac_register(0, 0, 0, 0);
    set_dac_register(1, 48, 0, 16);
    set_dac_register(2, 0, 48, 16);
    set_dac_register(3, 16, 0, 48);
    set_dac_register(4, 48, 16, 0);
*/    
/*    for (i=0; i<128; i++) {
    	set_dac_register(i, x*x, x*x, 32+((x*x)/2));
    	set_dac_register(i+128, 32+((x*x)/2), x*x, x*x);
    	x += dx;
    }
*/    
/*    
	    if (i > 111)
	    	set_dac_register(i, 32+((i-111)*2), (32+(i-111)*2), 16+i/4);
	    else
	    	set_dac_register(i, 32, 32, 16+i/4);

    for (i=0; i<128; i++)
	    if (i > 111)
	    	set_dac_register(i+128, 32+((i-111)*2), 16+i/4, 32+((i-111)*2));
	    else
	    	set_dac_register(i+128, 32, 16+i/4, 32);

*/

}
 
void destruct()
{
	kill_anim();
	killFaceHandler();
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

void wait_for_key(int ch)
{
     while (!kbhit())
     {
          if (kbhit())
               if (getch() == ch) return;
     }
}