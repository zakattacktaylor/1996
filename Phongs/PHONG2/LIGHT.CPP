/* LIGHT.CPP
*/
#include "types.h"
#include "math.h"
#include "light.h"

extern s16vector *light;
extern s16vector *pt2;

u8bit intensity()
{
	float average = 0;

	average += cos((pt2->x - light->x));
	average += cos((pt2->y - light->y));
	average += cos((pt2->z - light->z));
	average = average / 3;
	return average * 256;
}