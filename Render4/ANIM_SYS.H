#ifndef _ANIM_H
#define _ANIM_H

/* Frame Animation Header */
#include "types.h"
#include "vector.h"

extern u8bit *out_buff;
extern tScalar *z_buff;

void init_anim(void);
void flip_page(void);
void kill_anim(void);
void output(int n, char far *label, int len);
void set_dac_register(u8bit i, u8bit r, u8bit g, u8bit b);

#define vga_mode 1
#define screen_width 320
#define screen_height 200

#define x_focus 320
#define y_focus 200

#endif

