/* LINE.CPP
*/
#include "types.h"
#include "line.h"
extern "C" {
	#include "modex.h"
}

#define y_top_clip 399
#define y_bottom_clip 0
#define x_right_clip 319
#define x_left_clip 0

void trace_2d(s16bit x1, s16bit y1, s16bit x2, s16bit y2, s16bit *buff)
{
	s16bit xdiff,ydiff;
	s16bit xunit,yunit;
	s16bit xindex,yindex;
	s16bit error_term,length;
	s16bit i;

	yindex = y1;
	xindex = x1;

	ydiff = y2 - y1;
	if (ydiff<0) {
		ydiff=-ydiff;
		yunit=-1;
	}
	else yunit=1;

	xdiff = x2 - x1;
	if (xdiff<0) {
		xdiff=-xdiff;
		xunit=-1;
	}
	else xunit=1;

	error_term=0;

	if (xdiff>ydiff) {
		length=xdiff+1;
		if ((yindex>=y_bottom_clip)&&(yindex<=y_top_clip))
			buff[yindex]=xindex;
		for (i=0;i<length;i++) {
			xindex+=xunit;
			error_term+=ydiff;
			if (error_term>xdiff) {
				error_term-=xdiff;
				yindex+=yunit;
				if ((yindex>=y_bottom_clip)&&(yindex<=y_top_clip))
					buff[yindex]=xindex;
			}
		}
	}
	else {
		length=ydiff+1;
		if ((yindex>=y_bottom_clip)&&(yindex<=y_top_clip))
			buff[yindex]=xindex;
		for (i=0;i<length;i++) {
			yindex+=yunit;
			if ((yindex>=y_bottom_clip)&&(yindex<=y_top_clip))
				buff[yindex]=xindex;
			error_term+=xdiff;
			if (error_term>0) {
				error_term-=ydiff;
				xindex+=xunit;
			}
		}
	}
}

void gouraud_block(s16bit x1, s16bit y1, s16bit x2, s16bit y2, s16bit screen_y)
{
	s16bit xdiff,ydiff;
	s16bit xunit,yunit;
	s16bit xindex,yindex;
	s16bit error_term,length;
	s16bit i;

	yindex = y1;
	xindex = x1;

	ydiff = y2 - y1;
	if (ydiff<0) {
		ydiff=-ydiff;
		yunit=-1;
	}
	else yunit=1;

	xdiff = x2 - x1;
	if (xdiff<0) {
		xdiff=-xdiff;
		xunit=-1;
	}
	else xunit=1;

	error_term=0;

	if (xdiff>ydiff) {
		length=xdiff+1;
		if ((yindex>=x_left_clip)&&(yindex<=x_right_clip))
			set_point(yindex, screen_y, xindex);
		for (i=0;i<length;i++) {
			xindex+=xunit;
			error_term+=ydiff;
			if (error_term>xdiff) {
				error_term-=xdiff;
				yindex+=yunit;
				if ((yindex>=x_left_clip)&&(yindex<=x_right_clip))
					set_point(yindex, screen_y, xindex);
			}
		}
	}
	else {
		length=ydiff+1;
		if ((yindex>=x_left_clip)&&(yindex<=x_right_clip))
			set_point(yindex, screen_y, xindex);
		for (i=0;i<length;i++) {
			yindex+=yunit;
			if ((yindex>=x_left_clip)&&(yindex<=x_right_clip))
				set_point(yindex, screen_y, xindex);
			error_term+=xdiff;
			if (error_term>0) {
				error_term-=ydiff;
				xindex+=xunit;
			}
		}
	}
}
