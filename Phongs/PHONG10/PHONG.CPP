#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <iostream.h>
extern "C" {
	#include "modex.h"
}
#include "types.h"
#include "shape.h"
#include "anim_sys.h"
#include "vector.h"
#include "light.h"
#include "polygons.h"
#include <math.h>
#include <dos.h>
#include "keyboard.h"
#include "square_t.h"

boolean shut_down = no;
boolean space = no;

#include "cube.dat"
//#include "ring.dat"
#include "diamond.dat"

void main()
{
	shape *tube;
	vector *p_stube;
	s16bit *s_stube;

	init_sincos();
	init_light(40,40,40);

	init_shapes(1);

	#define num 17
	#define rings 9
	p_stube = stube_point(p_stube, num, rings, 20, 20, 5);
	s_stube = stube_poly(s_stube, num, rings);
	tube = new_shape(num*rings, (num*rings)-rings, &p_stube[0], &s_stube[0], 0, 0, 300);

	init_polygons((num*rings)-rings);

	init_anim();

	twist_light(16,64,0);

	while (shut_down == no)
	{
		if (space) wait_for_key(' ');
		if (kbhit()) key_action();

		draw_shapes();

		//if (tube->z > 0) tube->z -= 10;
		tube->xan++;
		tube->yan--;
		tube->zan--;

		flip_page();

		twist_light(-1,2,1);
	}

	kill_anim();
}