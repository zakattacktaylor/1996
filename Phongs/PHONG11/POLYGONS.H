/* POLYGONS.H
*/

void init_polygons(s16bit number);
void add_polygons(u16bit polygons, s16bit *polys, vector *r_points);

void sort_polygons(void);
void draw_polygons(void);

s16bit colorndistance(u8bit diffuse);
void polygon_color(s16bit *poly, vector *point);

void gouraud_polygon(s16bit *poly, vector *point);
void fill_polygon(s16bit *poly, vector *point, u8bit color);
void line_polygon(s16bit *poly, vector *point, u8bit color);
void shaded_line_polygon(s16bit *poly, vector *point);

