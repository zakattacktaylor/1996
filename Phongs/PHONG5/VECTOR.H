/* VECTOR.H
*/

/* prototypes */
void init_sincos(void);
void init_vectors(void);
void set_matrix(u8bit *xan, u8bit *yan, u8bit *zan);
void rotate(void);
void depth_emulate(void);
void compare(void);
void draw_point(u8bit *color);
void view_adjust(void);
u8bit on_screen(void);
u8bit visible(void);
void update_vectors(void);