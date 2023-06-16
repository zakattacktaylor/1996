/* LINE.CPP
*/
#include "types.h"
#include "line.h"
extern "C" {
	#include "modex.h"
}
extern s16vector *pt;

/* this routine does not perfrom any clipping */
void trace_2d(s16bit a, s16bit b, s16bit *buff)
{
	s16bit xdiff,ydiff;
	s16bit xunit,yunit;
	s16bit xindex,yindex;
	s16bit error_term,length;
	s16bit i;

	yindex = pt[a].y;
	xindex = pt[a].x;

	ydiff = pt[b].y - pt[a].y;
	if (ydiff<0) {
		ydiff=-ydiff;
		yunit=-1;
	}
	else yunit=1;

	xdiff = pt[b].x - pt[a].x;
	if (xdiff<0) {
		xdiff=-xdiff;
		xunit=-1;
	}
	else xunit=1;

	error_term=0;
	if (xdiff>ydiff) {
		length=xdiff+1;
		buff[yindex]=xindex;
		for (i=0;i<length;i++) {
			xindex+=xunit;
			error_term+=ydiff;
			if (error_term>xdiff) {
				error_term-=xdiff;
				yindex+=yunit;
				buff[yindex]=xindex;
			}
		}
	}
	else {
		length=ydiff+1;
		buff[yindex]=xindex;
		for (i=0;i<length;i++) {
			yindex+=yunit;
			buff[yindex]=xindex;
			error_term+=xdiff;
			if (error_term>0) {
				error_term-=ydiff;
				xindex+=xunit;
			}
		}
	}
}

void draw_2d(s16bit a, s16bit b, u8bit *color)
{
	s16bit xdiff,ydiff;
	s16bit xunit,yunit;
	s16bit xindex,yindex;
	s16bit error_term,length;
	s16bit i;

	yindex = pt[a].y;
	xindex = pt[a].x;

	ydiff = pt[b].y - pt[a].y;
	if (ydiff<0) {
		ydiff=-ydiff;
		yunit=-1;
	}
	else yunit=1;

	xdiff = pt[b].x - pt[a].x;
	if (xdiff<0) {
		xdiff=-xdiff;
		xunit=-1;
	}
	else xunit=1;

	error_term=0;
	if (xdiff>ydiff) {
		length=xdiff+1;
		set_point(xindex, yindex, *color);
		for (i=0;i<length;i++) {
			xindex+=xunit;
			error_term+=ydiff;
			if (error_term>xdiff) {
				error_term-=xdiff;
				yindex+=yunit;
				set_point(xindex, yindex, *color);
			}
		}
	}
	else {
		length=ydiff+1;
		set_point(xindex, yindex, *color);
		for (i=0;i<length;i++) {
			yindex+=yunit;
			set_point(xindex, yindex, *color);
			error_term+=xdiff;
			if (error_term>0) {
				error_term-=ydiff;
				xindex+=xunit;
			}
		}
	}
}
