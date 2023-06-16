#include <stdlib.h>
#include <dos.h>
#include <iostream.h>
#include <math.h>
#include "types.h"
#include "vector.h"
#include "square_t.h"
#include "polytype.def"

extern float tcos[256];

#define diffuse 2
#define specular 123
#define diffuse2 130
#define specular2 123

vector *stube_point(vector *p_stube, u16bit length, u16bit rings, s16bit radius1, s16bit radius2)
{
	u16bit i;
	vector *p;
	vector wp;
	u8bit xan, yan, zan;
	u8bit gxan, gyan, gzan;
	s16bit j;

	p_stube = (vector*)calloc(rings*length, sizeof(vector));
	if (p_stube == NULL)
	{
		cout << "out of mem -> stube_point\n";
		exit(1);
	}

	xan = 0;
	yan = 0;
	zan = 0;
	gxan = 0;
	gyan = 0;
	gzan = 0;

	p = &p_stube[0];

	for (i = 0; i < length; i++)
	{
		yan = 0;
/*		gxan += 5 * tcos[(u8bit)(i*256/(length*2))];
		gyan += 5 * tcos[(u8bit)(i*256/(length*2))];
		gzan += 5 * tcos[(u8bit)(i*256/(length*2))];
*/		//gxan += 256/(length-1);
		//gyan += 256/(length-1);
		gzan += 256/(length-1);
		//xan += 256/(length-1);
		//zan += 256/(length-1);
		for (j = 0; j < rings; j++)
		{
			p->x = radius1;
			p->y = radius1;
			p->z = radius1;

			yan += 256/(rings-1);

			set_matrix(&xan, &yan, &zan);
			rotate(p, &wp);

			wp.x += radius2;
			//wp.y += radius2;
			//wp.z += radius2;

			set_matrix(&gxan, &gyan, &gzan);
			rotate(&wp, p);

			p++;
		}
	}
	return &p_stube[0];
}

s16bit *stube_poly(s16bit *s_stube, u16bit length, u16bit rings)
{
	u16bit i;
	s16bit k,j,k1,k2;
	s16bit *poly;

	s_stube = (s16bit*)calloc(2*10*length*(rings-1), sizeof(s16bit));
	if (s_stube == NULL)
	{
		cout << "out of mem\n";
		exit(1);
	}
	poly = &s_stube[0];
	for (i = 0; i < (length - 1); i++)
	{

		k1 = i * rings;
		k2 = (i + 1) * rings;
		for (j = 0; j < (rings-1); j++)
		{
			*poly = shaded;
			poly++;
			*poly = reverse;
			poly++;
			*poly = diffuse;
			poly++;
			*poly = specular;
			poly++;
			*poly = 0;
			poly++;
			*poly = 0;
			poly++;
			*poly = k1 + j;
			poly++;
			*poly = k1 + j + 1;
			poly++;
			*poly = k2 + j + 1;
			poly++;
			*poly = k1 + j;
			poly++;
			*poly = shaded;
			poly++;
			*poly = reverse;
			poly++;
			*poly = diffuse2;
			poly++;
			*poly = specular2;
			poly++;
			*poly = 0;
			poly++;
			*poly = 0;
			poly++;
			*poly = k1 + j;
			poly++;
			*poly = k2 + j;
			poly++;
			*poly = k2 + j + 1;
			poly++;
			*poly = k1 + j;
			poly++;
		}
	}
	return &s_stube[0];
}