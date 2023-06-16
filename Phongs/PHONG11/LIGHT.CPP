/* LIGHT.CPP
*/
#include <iostream.h>
#include "types.h"
#include "vector.h"
#include "light.h"

extern float tcos[256];

#define diffuse 0
#define specular 126

vector *light;
u8bit xan;
u8bit yan;
u8bit zan;

void init_light(void)
{
	light = new vector;
	light->x = specular;
	light->y = specular;
	light->z = specular;
	xan = 0;
	yan = 0;
	zan = 0;
}

void intensity(vector *p)
{
	float l = 0;
	u8bit c;

	l += (p->x - light->x);
	l += (p->y - light->y);
	l += (p->z - light->z);
	l = l / 3;
	c = l;
	if ((c >= 64)&&(c <= 192))
		p->color = diffuse;
	else
		p->color = (specular * tcos[c] * tcos[c] * tcos[c]* tcos[c] * tcos[c]* tcos[c]) + diffuse;
}

void twist_light(u8bit x, u8bit y, u8bit z)
{
	vector p;

	xan += x;
	yan += y;
	zan += z;
	p.x = specular;
	p.y = specular;
	p.z = specular;
	set_matrix(&xan, &yan, &zan);
	rotate(&p, light);
}
