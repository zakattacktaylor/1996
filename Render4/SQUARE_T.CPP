#include <stdlib.h>
#include <dos.h>
#include <iostream.h>
#include "types.h"
#include "vector.h"
#include "square_t.h"
#include "polytype.def"

#define diffuse 2
#define diffuse2 128
#define specular 125

vector *stube_point(vector *p_stube, u16bit length, u16bit rings, s16bit x, s16bit y, s16bit z_size)
{
	u16bit i;
	vector *p;
	vector p1;
	s16bit z;
	s16bit *poly;
	u8bit xan, yan, zan;
	s16bit j;

	p_stube = (vector*)calloc(rings*length, sizeof(vector));
	if (p_stube == NULL)
	{
		cout << "out of mem\n";
		exit(1);
	}

	xan = 0;
	yan = 0;
	zan = 0;
	p = &p_stube[0];
	for (i = 0; i < length; i++)
	{
		z = (i+1) * z_size;
		zan += 256 / (length - 1);
		yan = 0;
                //xan += 2;
		for (j = 0; j < rings; j++)
		{
			p->x = x;
			p->y = y;
			p->z = 0;
			yan += 256/(rings-1);
			//xan++;
			set_matrix(&xan, &yan, &xan);
			rotate(p, &p1);
			p1.x += 80;
			//xan++;
			set_matrix(&zan, &xan, &zan);
			rotate(&p1, p);
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
			//*poly = gouraud;
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
			//*poly = gouraud;
			*poly = shaded;
			poly++;
			*poly = reverse;
			poly++;
			*poly = diffuse2;
			poly++;
			*poly = specular;
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