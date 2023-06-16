void print_s32(long *a, int x, int y)
{
	char s1[15];
	itoa(focus,s1,15);
	print_str(s1,3,20,220,200,0);
}

void put_hud(void)
{
#define tsize 10
	char s1[tsize];
	char s2[tsize];
	char s3[tsize];
	char s4[tsize];
	char s5[tsize];
	char s6[tsize];
	char s7[tsize];

	ltoa(focus,s1,tsize);
	ltoa(camera.x,s2,tsize);
	ltoa(camera.z,s3,tsize);
	ultoa(camera.xan,s4,tsize);
	ultoa(camera.yan,s5,tsize);
	ultoa(camera.zan,s6,tsize);
	ltoa(polys_drawn,s7,tsize);

	print_str(s1,tsize,0,220,200,0);
	print_str(s2,tsize,50,220,202,0);
	print_str(s3,tsize,100,220,204,0);
	print_str(s4,tsize,150,220,206,0);
	print_str(s5,tsize,200,220,208,0);
	print_str(s6,tsize,250,220,210,0);
	print_str(s7,tsize,300,220,212,0);
}
