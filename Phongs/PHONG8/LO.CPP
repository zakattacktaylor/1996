/* LIGHT.CPP
*/
#include "types.h"
#include "math.h"
#include "light.h"

extern s16vector *light;
extern s16vector *pt;
extern s16vector *pt2;

extern float tcos[256];

u8bit intensity()
{
	float average = 0;

	average += 256 * abs(pt2->x - light->x) / 360;
	average += 256 * abs(pt2->y - light->y) / 360;
	average += 256 * abs(pt2->z - light->z) / 360;
	average = average / 3;
	return 256 * tcos[(u8bit)average];
}