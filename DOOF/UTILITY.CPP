void init_tables(void);
void swap(int*, int*);
int on_screen(int, int);

void init_tables()
{
	int i;

	for (i=0;i<maxpolys;i++) tentimes[i]=10*i;
	for (i=0;i<maxpoints;i++) threetimes[i]=3*i;
}

void swap(int *a,int *b)
{
	int t;
	t=*a;
	*a=*b;
	*b=t;
}

int on_screen(int x, int y)
{
	if (x<0) return 0;
	if (y<0) return 0;
	if (x>screen_width) return 0;
	if (y>screen_height) return 0;
	return 1;
}
