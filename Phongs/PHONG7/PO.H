void line_array(void)
{
	int xdiff,ydiff;
	int xunit,yunit;
	int xindex,yindex;
	int error_term,length;
	int i;

	yindex=y1;
	xindex=x1;

	ydiff=y2-y1;
	if (ydiff<0) {
		ydiff=-ydiff;
		yunit=-1;
	}
	else yunit=1;

	xdiff=x2-x1;
	if (xdiff<0) {
		xdiff=-xdiff;
		xunit=-1;
	}
	else xunit=1;

	error_term=0;
	if (xdiff>ydiff) {
		length=xdiff+1;
		if ((yindex>=y_bottom_clip)&&(yindex<=y_top_clip))
			lineme[yindex]=xindex;
		for (i=0;i<length;i++) {
			xindex+=xunit;
			error_term+=ydiff;
			if (error_term>xdiff) {
				error_term-=xdiff;
				yindex+=yunit;
				if ((yindex>=y_bottom_clip)&&(yindex<=y_top_clip))
					lineme[yindex]=xindex;
			}
		}
	}
	else {
		length=ydiff+1;
		if ((yindex>=y_bottom_clip)&&(yindex<=y_top_clip))
			lineme[yindex]=xindex;
		for (i=0;i<length;i++) {
			yindex+=yunit;
			if ((yindex>=y_bottom_clip)&&(yindex<=y_top_clip))
				lineme[yindex]=xindex;
			error_term+=xdiff;
			if (error_term>0) {
				error_term-=ydiff;
				xindex+=xunit;
			}
		}
	}
}

void line_array2(void)
{
	int xdiff,ydiff;
	int xunit,yunit;
	int xindex,yindex;
	int error_term,length;
	int i;

	yindex=y1;
	xindex=x1;

	ydiff=y2-y1;
	if (ydiff<0) {
		ydiff=-ydiff;
		yunit=-1;
	}
	else yunit=1;

	xdiff=x2-x1;
	if (xdiff<0) {
		xdiff=-xdiff;
		xunit=-1;
	}
	else xunit=1;

	error_term=0;
	if (xdiff>ydiff) {
		length=xdiff+1;
		if ((yindex>-1)&&(yindex<screen_width-1))
			lineme[yindex]=xindex;
		for (i=0;i<length;i++) {
			xindex+=xunit;
			error_term+=ydiff;
			if (error_term>xdiff) {
				error_term-=xdiff;
				yindex+=yunit;
				if ((yindex>=0)&&(yindex<screen_width-1))
					lineme[yindex]=xindex;
			}
		}
	}
	else {
		length=ydiff+1;
		if ((yindex>=0)&&(yindex<screen_width))
			lineme[yindex]=xindex;
		for (i=0;i<length;i++) {
			yindex+=yunit;
			if ((yindex>-1)&&(yindex<screen_width))
				lineme[yindex]=xindex;
			error_term+=xdiff;
			if (error_term>0) {
				error_term-=ydiff;
				xindex+=xunit;
			}
		}
	}
}

void set_lambert(void)
{
	int t;

	lineme=&x_zarr[0];
	t=threetimes[sides[maxxindex]];
	x1=points[t+2];
	y1=points[t];
	t=threetimes[sides[minxindex]];
	x2=points[t+2];
	y2=points[t];
	line_array2();
	lineme=&y_zarr[0];
	t=threetimes[sides[maxyindex]];
	x1=points[t+2];
	y1=points[t+1];
	t=threetimes[sides[minyindex]];
	x2=points[t+2];
	y2=points[t+1];
	line_array();
}

void fill_poly(void)
{
	int this_side,next_side;
	int* arr;
	int* ar2;
	int sides_done=0;
	int i;
	int c,y_val;

	this_side=maxyindex;
	next_side=maxyindex+1;
	if (next_side>3) next_side=0;
	lineme=&array_one[0];

line_array:
	i=threetimes[sides[this_side]];
	x1=points[i];
	y1=points[i+1];
	i=threetimes[sides[next_side]];
	x2=points[i];
	y2=points[i+1];

	line_array();
	sides_done++;
	if (y2<=miny) lineme=&array_two[0];
	this_side=next_side;
	next_side++;
	if (next_side>=4) next_side=0;
	if (sides_done==4) goto poly_fill;
	else goto line_array;

poly_fill:
	y1=miny;
	if (y1<y_bottom_clip) y1=y_bottom_clip;
	arr=&array_one[y1];
	ar2=&array_two[y1];
	if (maxy>y_top_clip) maxy=y_top_clip;
loop_line:
	x1=*arr;
	x2=*ar2;
	arr++;
	ar2++;

	if (x1<x_left_clip) x1=x_left_clip;
	if (x1>x_right_clip) x1=x_right_clip;
	if (x2<x_left_clip) x2=x_left_clip;
	if (x2>x_right_clip) x2=x_right_clip;
	if (x1!=x2) fill_block(x1,y1,x2,y1,poly_color);

	y1++;
	if (y1>=maxy) return;
	goto loop_line;
}