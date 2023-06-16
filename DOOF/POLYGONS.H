//global setup!
//poly_color=the color
//poly_sides=sides in poly
//points=array[3*poly_sides]
//sides=array[poly_sides]
//maxy=maximum y point
//maxyindex=max y index
//miny=minimum y point

#define y_top_clip 120

int poly_color;
int poly_sides=4;
int sides[4];
int* points;
int maxy;
int maxyindex;
int minyindex;
int miny;
int maxx;
int maxxindex;
int minx;
int minxindex;
int* lineme;
int array_one[screen_height];
int array_two[screen_height];
int x_zarr[screen_width];
int y_zarr[screen_height];

int x1,y1,x2,y2;

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
                if ((yindex>-1)&&(yindex<y_top_clip))
                        lineme[yindex]=xindex;
                for (i=0;i<length;i++) {
                        xindex+=xunit;
                        error_term+=ydiff;
                        if (error_term>xdiff) {
                                error_term-=xdiff;
                                yindex+=yunit;
                                if ((yindex>=0)&&(yindex<y_top_clip))
                                        lineme[yindex]=xindex;
                        }
                }
        }
        else {
                length=ydiff+1;
                if ((yindex>=0)&&(yindex<y_top_clip))
                        lineme[yindex]=xindex;
                for (i=0;i<length;i++) {
                        yindex+=yunit;
                        if ((yindex>-1)&&(yindex<y_top_clip))
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
        if (y1<0) y1=0;
        arr=&array_one[y1];
        ar2=&array_two[y1];
        if (maxy>y_top_clip) maxy=y_top_clip;
loop_line:
        x1=*arr;
        x2=*ar2;
        arr++;
        ar2++;
        
        if (x1<0) x1=0;
        if (x1>=screen_width) x1=screen_width-1;
        if (x2<0) x2=0;
        if (x2>=screen_width) x2=screen_width-1;
        
        //if (x1>x2) swap(&x2,&x1);
        
        /*
        y_val=y_zarr[y1];
        for (i=x1;i<=x2;i++)
        {
                c=x_zarr[i]+y_val;
                c=(c>>2)+64;
                if (c>127) c=127;
                if (c<0) c=-c;
                set_point(i,y1+1,c+128);
                set_point(i,y_top_clip+(y_top_clip-y1),c);
        }
        */
        fill_block(x1,y1,x2,y1,50);
        y1++;
        if (y1>maxy) return;
        goto loop_line;
}
