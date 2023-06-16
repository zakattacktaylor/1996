/* Frame Animation Routines */

#include "types.h"
#include "anim_sys.h"
extern "C" {
#include "modex.h"
}
#include <time.h>
#include <stdio.h>

   //////////////////
 /* VGA MODES    //
/////////////////
 Mode #  Screen Size    Max Pages   Aspect Ratio (X:Y)

    0      320 x 200      4 Pages         1.2:1
    1      320 x 400      2 Pages         2.4:1
    2      360 x 200      3 Pages        1.35:1
    3      360 x 400      1 Page          2.7:1
    4      320 x 240      3 Pages           1:1
    5      320 x 480      1 Page            2:1
    6      360 x 240      3 Pages       1.125:1
    7      360 x 480      1 Page         2.25:1
*/

/* Globals */
u32bit frames=0;
u32bit frame_time;
u8bit pg_now;

u8bit palette[256][3];

void init_anim()
{
	s16bit i;

	set_modex(vga_mode);
	pg_now=0;
	set_display_page(0);
	set_active_page(1);
	clear_vga_screen(0);

	for (i = 0; i < 32; i++)
		set_dac_register(i+32, 32, 2*i, 2*i);

	for (i = 0; i < 32; i++)
		set_dac_register(i+64, i, 32+i, 2*i);

	for (i = 0; i < 32; i++)
		set_dac_register(i+128, 2*i, 2*i, i+16);

	for (i = 0; i < 32; i++)
		set_dac_register(i+128+64+32, 2*i, 2*i, 63);

	frame_time=time(NULL);
}

void flip_page()
{
	if (pg_now)
	{
		set_display_page(0);
		set_active_page(1);
		clear_vga_screen(0);
		pg_now=0;
	}
	else
	{
		set_display_page(1);
		set_active_page(0);
		clear_vga_screen(0);
		pg_now=1;
	}
	frames++;
}

void kill_anim()
{
	_asm mov ax,03h
	_asm int 10h
	printf("%d frames per second\n",frames/(time(NULL)-frame_time));
}

