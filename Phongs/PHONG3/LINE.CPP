/* LINE.CPP
*/
#include "types.h"
#include "line.h"

#define screen_height 400

s16bit x_left[screen_height];
s16bit x_right[screen_height];
s16bit z_left[screen_height];
s16bit z_right[screen_height];

/* this routine does not perfrom any clipping */
void trace_2d(s16vector *p1, s16vector *p2, s16bit *buff)
{
	s16bit xdiff,ydiff;
	s16bit xunit,yunit;
	s16bit xindex,yindex;
	s16bit error_term,length;
	s16bit i;

	yindex = p1->y;
	xindex = p1->x;

	ydiff = p2->y - p1->y;
	if (ydiff<0) {
		ydiff=-ydiff;
		yunit=-1;
	}
	else yunit=1;

	xdiff = p2->x - p1->x;
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
