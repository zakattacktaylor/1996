/* LIGHT.CPP
*/
#include "types.h"
#include "math.h"
#include "light.h"

extern s16vector *light;
extern s16vector *pt;
extern s16vector *pt2;

extern float l_xan;
extern float l_yan;
extern float l_zan;

extern float tcos[256];

u8bit intensity()
{
	register float x, y, z;
	register u8bit p_xan, p_yan, p_zan;

	x = pt2->x + 0.001;
	y = pt2->y + 0.001;
	z = pt2->z + 0.001;

	p_xan = abs(atan2(z, y) - l_xan) * 127 / M_PI;
	p_yan = abs(atan2(x, z) - l_yan) * 127 / M_PI;
	p_zan = abs(atan2(y, x) - l_zan) * 127 / M_PI;

	if (p_xan > 63) p_xan = 63;
	if (p_yan > 63) p_yan = 63;
	if (p_zan > 63) p_zan = 63;

	x = 0;
	x += tcos[p_xan] * 256 / 3;
	x += tcos[p_yan] * 256 / 3;
	x += tcos[p_zan] * 256 / 3;

	if (x >= 255) return 255;
	if (x <= 0) return 0;

	return  (u8bit)x;
}