/* VECTOR.H
*/

/* prototypes */
void init_sincos(void);
vector *init_vectors(u16bit number, vector *data);
void set_matrix(u8bit *xan, u8bit *yan, u8bit *zan);
void rotate(void);
void depth_emulate(void);
void compare(void);
void draw_point(vector *pt);
void view_adjust(void);
u8bit on_screen(void);
u8bit visible(void);
void update_vectors(void);