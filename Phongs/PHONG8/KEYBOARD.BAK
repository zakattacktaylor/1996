/* KEYBOARD.H
*/

#include <conio.h>
#include "types.h"
#include "keyboard.h"

void wait_for_key(int ch)
{
	while (!kbhit())
	{
		if (kbhit())
			if (getch() == ch) return;
	}
}
