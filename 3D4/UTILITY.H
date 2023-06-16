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