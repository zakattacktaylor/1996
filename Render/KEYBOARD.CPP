/* KEYBOARD.H
*/

#include <conio.h>
#include "types.h"
#include "keyboard.h"

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
