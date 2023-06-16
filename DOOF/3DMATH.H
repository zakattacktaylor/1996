#include "sincos.h"

#define a_sin(a) sintbl[a]
#define a_cos(a) costbl[a]
#define pi 3.1452
#define nin 1.57260
#define nin2 4.71780
#define nin3 6.29040
long ratiox=400000/320;
long ratioy=400000/320;


void set_matrix(void)
{
	cx=a_cos(gxan);
	sx=a_sin(gxan);
	cy=a_cos(gyan);
	sy=a_sin(gyan);
	cz=a_cos(gzan);
	sz=a_sin(gzan);
}


void clip_angles(int* x,int* y,int* z)
{
	while (*x<0) *x+=360;
	while (*x>359) *x-=360;
	while (*y<0) *y+=360;
	while (*y>359) *y-=360;
	while (*z<0) *z+=360;
	while (*z>359) *z-=360;
}

void clip_angle(int* x)
{
	while (*x<0) *x+=360;
	while (*x>359) *x-=360;
}

void depth_emul(void)
{
	long ex=gx*ratiox;
	long ey=gy*ratioy;
	if (gz<0) gz=-gz;
	if (gz==0) gz=1;
	gx=ex/gz;
	gy=ey/gz;
}

void rotate_pt(void)
{
	long xt,yt,zt;

	yt=(ly*cx-lz*sx) >> 8;
	zt=(ly*sx+lz*cx) >> 8;
	ly=yt;
	lz=zt;

	xt=(lx*cy-lz*sy) >> 8;
	zt=(lx*sy+lz*cy) >> 8;
	lx=xt;
	lz=zt;

	xt=(lx*cz-ly*sz) >> 8;
	yt=(lx*sz+ly*cz) >> 8;
	lx=xt;
	ly=yt;

}

long distance(long x1,long y1,long z1,long x2,long y2,long z2)
{
	long temp=((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2))+((z1-z2)*(z1-z2));
	return((long)sqrtl(temp));
}
