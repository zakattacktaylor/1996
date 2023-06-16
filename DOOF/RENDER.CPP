#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "def_glob.h"
#include "modex.h"
#include "utility.h"
#include "anim_sys.h"
#include "polygons.h"
#include "3dmath.h"
#include "data.h"
#include "objhandl.h"

void main()
{
	int x=0,y=0,z=0;
	int i;
	int timetogo=0;
	char key;

	init_obj_sys(1);
	init_object(0,6,8,&cubesdata2[0],&cubedata2[0],0,-65,-3200);

	set_cam_xyz(x,y,z);
	init_tables();

	init_anim();

	for (i=0;i<64;i++) {
		set_dac_register(i,0,0,i);              //black-blue
		set_dac_register(i+64,0,i,63-i);        //blue-green
		set_dac_register(i+128,i,0,i/2);
		set_dac_register(i+192,63-i/4,i,32-i/2);
	}

	timetogo=0;
	while (!timetogo) {
		if (kbhit()) {
			key=getch();
			if (key==27) timetogo=1;
		}

		twist_object(0,2,3,1);
		put_object(0);
		
		flip_page();                
		
	}
	kill_obj_sys();
	kill_anim();
}

