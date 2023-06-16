void init_tables(void)
{
	int i;

	for (i=0;i<maxpolys;i++) tentimes[i]=10*i;
	for (i=0;i<maxpoints;i++) threetimes[i]=3*i;
}

void move_it(int *a,int *aer,int min,int max)
{
	if ((*a<min)||(*a>max)) {
		if (*aer<0) {
			*aer=-*aer;
			*a=min;
		}
		else {
			*aer=-*aer;
			*a=max;
		}
	}
	else *a+=*aer;
}

void swap(int *a,int *b) {
	int t;
	t=*a;
	*a=*b;
	*b=t;
}

int the_max(int a1,int a2,int a3,int a4)
{
	int max=a1;
	if (a2>max) max=a2;
	if (a3>max) max=a3;
	if (a4>max) max=a4;
	return(max);
}

int the_min(int a1,int a2,int a3,int a4)
{
	int min=a1;
	if (a2<min) min=a2;
	if (a3<min) min=a3;
	if (a4<min) min=a4;
	return(min);
}
 
float absf(float value) 
{
	return (value < 0) ? -value : value;
}

int on_screen(int x,int y)
{
	if (x<0) return 0;
	if (y<0) return 0;
	if (x>screen_width) return 0;
	if (y>screen_height) return 0;
	return 1;
}
