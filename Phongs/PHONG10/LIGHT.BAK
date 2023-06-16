/* LIGHT.CPP
*/
#include <iostream.h>
#include "types.h"
#include "vector.h"
#include "light.h"

extern float tcos[256];

vector *light;
u8bit xan;
u8bit yan;
u8bit zan;

void init_light(s16bit x, s16bit y, s16bit z)
{
	light = new vector;
	light->x = x;
	light->y = y;
	light->z = z;
	xan = 32;
	yan = 32;
	zan = 32;
}

void intensity(vector *p, u8bit ambient, u8bit shades)
{
	float l = 0;
	u8bit c;

	l += (p->x - light->x);
	l += (p->y - light->y);
	l += (p->z - light->z);
	l = l / 3;
	c = l;
	if ((c >= 64)&&(c <= 192))
		p->color = ambient;
	else
		p->color = (shades * tcos[c]) + ambient;
}

void twist_light(u8bit x, u8bit y, u8bit z)
{
	vector p;

	xan += x;
	yan += y;
	zan += z;
	light->x = 40;
	light->y = 40;
        light->z = 40;
	set_matrix(&xan, &yan, &zan);
	rotate(light, &p);
	light->x = p.x;
	light->y = p.y;
	light->z = p.z;
}
