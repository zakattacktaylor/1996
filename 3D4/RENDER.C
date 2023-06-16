#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "def_glob.h"
#include "modex.h"
#include "utility.h"
#include "polygons.h"
#include "animsys.h"
#include "3dmath.h"
#include "data.h"
#include "objhandl.h"
#include "hud.h"

void main()
{
	int i;
	int timetogo=0;
	char key;
#define number 1
#define camera_chg 100
	init_obj_sys(number);
	for(i=0;i<number;i++)
	{
		init_object(i,6,8,&cubesdata[0],&cubedata[0],30*i*i,30*i*i,(i*i*200)+40000);
	}

	set_cam_xyz(0,0,0);
	init_tables();

	init_anim();

	for (i=0;i<64;i++) {
		set_dac_register(i,0,0,i);              //black-blue
		set_dac_register(i+64,0,i,63-i);        //blue-green
		set_dac_register(i+128,i/2,63-i/2,0);
		set_dac_register(i+192,32,32+i/2,i);
		//set_dac_register(i+128,0,2*i,63-(2*i));
		//set_dac_register(i+160,63-i/2,2*i,32-i);
		//set_dac_register(i+192,0,2*i,63-(2*i));
		//set_dac_register(i+224,0,2*i,63-(2*i));
	}
	space=0;
	timetogo=0;
	while (!timetogo)
	{
		if (kbhit())
		{
			key=getch();
			if (!key)
			{
			key=getch();
			switch (key)
			{
#define cmove 10
				case 77:
					camera.yan+=cmove;
					break;
				case 75:
					camera.yan-=cmove;
					break;
				case 72:
					camera.xan+=cmove;
					break;
				case 80:
					camera.xan-=cmove;
					break;
				case 115:
					camera.zan+=cmove;
					break;
				case 116:
					camera.zan-=cmove;
					break;
				}
			}
			else
			switch (key)
			{
				case 13:
					if (space) space=0;
					else space=1;
					break;
				case 27:
					timetogo=1;
					break;
				case 'a':
					focus+=foc_chg;
					if (focus>foc_max) focus-=foc_chg;
					break;
				case 'z':
					focus-=foc_chg;
					if (focus<foc_min) focus+=foc_chg;
					break;
				case 'i':
					camera.y+=camera_chg;
					break;
				case 'm':
					camera.y-=camera_chg;
					break;
				case 'j':
					camera.x+=camera_chg;
					break;
				case 'k':
					camera.x-=camera_chg;
					break;
				case 'd':
					camera.z+=camera_chg;
					break;
				case 'c':
					camera.z-=camera_chg;
					break;
			}
		}

		for(i=(number-1);i>=0;i--)
		{
			twist_object(i,0,0,0);
			put_object(i);
		}
		sort_polys();
                do_update();
		put_hud();
		nextpoly=0;
		flip_page();
		polys_drawn=0;

	}
	kill_obj_sys();
	kill_anim();
}

