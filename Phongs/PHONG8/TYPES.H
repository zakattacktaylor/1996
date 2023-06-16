/* TYPES */
typedef unsigned long u32bit;
typedef long s32bit;
typedef unsigned int u16bit;
typedef int s16bit;
typedef unsigned char u8bit;
typedef char s8bit;
typedef char boolean;

#define yes 1
#define no 0

/* VECTORS */
typedef struct { u32bit x,y,z; } u32vector;
typedef struct { s32bit x,y,z; } s32vector;
typedef struct { u16bit x,y,z; } u16vector;
typedef struct { s16bit x,y,z; } s16vector;
typedef struct { u8bit x,y,z; } u8vector;
typedef struct { s8bit x,y,z; } s8vector;

/* POINTS */
typedef struct { s16bit x,y,z;
		 u8bit color; } vector;

/* COLOR VECTOR */
typedef struct { u8bit r,g,b; } rgbcolor;

/* LINES */
typedef struct { s32bit p1,p2; } line;