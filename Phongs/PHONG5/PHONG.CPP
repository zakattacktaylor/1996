#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
extern "C" {
	#include "modex.h"
}
#include "types.h"
#include "anim_sys.h"
#include "vector.h"
#include "light.h"
#include "polygons.h"
#include <math.h>
#include <dos.h>

extern u8bit xan;
extern u8bit yan;
extern u8bit zan;
extern s16bit z_offset;

void main()
{
	boolean shut_down = no;
	u8bit key;
	s16bit i,j,n;
	s16bit *depth;
	s16bit *index;

	init_sincos();
	init_vectors();
	init_polygons();
	init_anim();

	while (shut_down == no)
	{
		if (kbhit())
		{
			key = getch();
			switch (key)
			{
				case 27:
					shut_down = yes;
					break;
				case 'a':
					z_offset += 10;
					break;
				case 'z':
					z_offset -=10;
					break;
				default:
					break;
			}
		}

		update_vectors();
		update_polygons();

		flip_page();

		xan++;
		yan++;
		zan++;
	}

	kill_anim();
}