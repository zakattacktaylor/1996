void init_stars(void);
void calc_stars(void);
void put_stars(void);

void init_stars()        
{
        int x,y,z,k,i;
        for (i=0;i<maxstars;i++)
        {
                k=i+i+i;
                x=0;
                y=0;
                z=300;
                rotate_pt (&x,&y,&z,random(360),random(360),random(360));
                stars[k]=x;
                stars[k+1]=y;
                stars[k+2]=z;
        }
        STARS_ON=1;
}

void calc_stars()
{
        int x,y,z,k,i;
        
        for (i=0;i<maxstars;i++) {
                k=i+i+i;
                x=stars[k];
                y=stars[k+1];
                z=stars[k+2];
                rotate_pt(&x,&y,&z,camera.xan,camera.yan,camera.zan);
                ste[k]=x+160;
                ste[k+1]=y+120;
                ste[k+2]=z;
        }
}        

void put_stars()                
{
        int x,y,z,i;

        for (i=0;i<maxstars;i++)
        {
                x=ste[i+i+i];
                y=ste[i+i+i+1];
                z=ste[i+i+i+2];
                if (on_screen(x,y)==1)
                if (z>50) {
                        set_point(x,y,i>>1);
                }
        }
}
