/* Frame Animation Routines */

#include <stdlib.h>
#include "types.h"
#include "anim_sys.h"
#include <time.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>

unsigned long far *screen;
tScalar *z_buff;
u8bit *out_buff;

u32bit frames=0;
u32bit frame_time;
u8bit pg_now;
int output_y;

void to256(void)
{
	union REGS r;
	
	r.x.ax = 0x13;
	
	int86(0x10, &r, &r);
	outp(0x3c2, 0xe3);
}


void totxt(void)
{
	union REGS r;
	
	r.x.ax = 0x3;
	int86(0x10, &r, &r);
}

void set_dac_register(u8bit i, u8bit r, u8bit g, u8bit b)
{
	outp(0x3c8, i);

	outp(0x3c9, r);
	outp(0x3c9, g);
	outp(0x3c9, b);
}

void clear_vga_screen(u8bit c)
{
	int i;
	unsigned long cc;
	unsigned long far *p = screen;
	unsigned long *z = (unsigned long*)z_buff;
	unsigned long *o = (unsigned long*)out_buff;
	
	cc = (c << 24) + (c << 16) + (c << 8) + c;
		
	for (i=0; i<16000; i++) {
		*p = cc;
		*o = cc;
		p++;
		o++;
	}
}

void init_anim()
{
	unsigned int i;
	
	out_buff = new u8bit[64000];
	if (!out_buff) {
		printf("not enough mem for out_buffer\n");
		exit(0);
	}
	z_buff = new tScalar[64000];
	if (z_buff == 0) {
		printf("not enough mem for z_buff\n");
		exit(0);
	}	
	to256();
	screen = (unsigned long far*)0xa0000000;
	pg_now=0;
	clear_vga_screen(0);	
	frame_time=time(NULL);
	output_y = 0;
}

void flip_page()
{
	unsigned int i;
	unsigned long far *s = screen;
	unsigned long *b = (unsigned long*)out_buff;
	tScalar *z = z_buff;
	
	for (i=0; i<16000; i++) {
		*s = *b;
		*b = 0;
		s++;
		b++;
	}

	for (i=0; i<64000; i++) {
		*z = 0; //3.402823466e+38F;
		z++;
	}

	frames++;
	output_y = 0;
}

void kill_anim()
{
	totxt();
	printf("%d frames per second\n",frames/(time(NULL)-frame_time));
}

int next_output_y()
{
	int c = output_y;
	output_y += 20;
	return c;
}

void output(int n, char far *label, int len)
{
	char buff[10];
	int y = next_output_y();
		
	itoa(n, buff, 10);
	//tprint_str(label, len, 0, y, 100);
	//tprint_str(buff, 10, 10*len, y, 150);
}
