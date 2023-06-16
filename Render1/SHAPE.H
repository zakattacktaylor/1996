/* SHAPE.H
*/

typedef struct { s16bit x,y,z;
		 u8bit xan,yan,zan;
		 u16bit vertices;
		 u16bit polygons;
		 vector *points;
		 vector *r_points;
		 s16bit *polys;
	       } shape;

void init_shapes(u16bit number);
shape *new_shape(u16bit vertices, u16bit polygons, vector *points, s16bit *polys, s16bit x, s16bit y, s16bit z);
void draw_shapes();
