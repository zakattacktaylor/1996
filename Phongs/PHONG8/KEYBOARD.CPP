/* KEYBOARD.H
*/

#include <conio.h>
#include "types.h"
#include "keyboard.h"

extern s16bit x_offset;
extern s16bit y_offset;
extern s16bit z_offset;
extern boolean shut_down;
extern boolean space;

void wait_for_key(int ch)
{
	while (!kbhit())
	{
		if (kbhit())
			if (getch() == ch) return;
	}
}

void key_action()
{
	u8bit key;

	key = getch();
	switch (key)
	{
		case 27:
			shut_down = yes;
			break;
		case 'a':
			z_offset += 10;
			break;
		case 'z':
			z_offset -= 10;
			break;
		case 's':
			if (space)
				space = no;
			else
				space = yes;
			break;
		case 72:
			y_offset += 10;
			break;
		case 80:
			y_offset -= 10;
			break;
		case 77:
			x_offset += 10;
			break;
		case 75:
			x_offset -= 10;
			break;
		default:
			break;
	}
}
