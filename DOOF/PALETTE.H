int pal[256][3];
int ptop=255;
int pbot=0;
int pcenter=128;
int pcycle=0;
int pcycr=0;
int pcycg=0;
int pcycb=0;


void pal_set_cycle(void);
{
        ptop=random(256-pcenter)+pcenter;
        pbot=random(pcenter);
        pcenter=random(ptop-pbottom-1)+pbottom;
        pcycr=random(64);
        pcycr=random(64);
        pcycg=random(64);
        pcycle=1;


}

