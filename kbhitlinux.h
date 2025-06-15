#include <ncurses.h>

int kbhitL()
{
	initscr();
	cbreak();
	noecho();
	scrollok(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	return getch();
}
