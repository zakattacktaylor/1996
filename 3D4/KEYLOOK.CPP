#include <stdio.h>
#include <conio.h>

void main()
{
	int quit=0;
	char ch=0;
	char ech=0;
	while (!quit)
	{
		if (kbhit())
		{
			ch=getch();
			if (!ch) ech=getch();
			printf("ch=%ud ech=%ud\n",ch,ech);
			if (ch==27) quit=1;
			ech=0;
		}
	}
}