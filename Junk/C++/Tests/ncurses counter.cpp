#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <string>

WINDOW* win;
int main(int total, char** init)
{
	if (init) {
		win = initscr();
		if (win == NULL)
			exit(EXIT_FAILURE);
		total = 0;
		noecho();
	}

	mvaddstr(13, 33, std::to_string(total).c_str());
	refresh();

	int c = wgetch(win);
	if (isdigit(c)) {
		main(total + c - '0',0);
	}

	if (init) {
		delwin(win);
		endwin();
		refresh();
	}

	return EXIT_SUCCESS;
}
