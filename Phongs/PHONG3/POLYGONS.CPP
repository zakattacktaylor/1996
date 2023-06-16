/* POLYGONS.CPP
*/

extern u16bit *poly;
extern s16vector *pts

s16bit distance()
{
	s16bit i;
	s16bit z = 0;

	for (i = 0; i < 4; i++)
		z += pts[poly[i]].z;

	return z >> 2;
}

