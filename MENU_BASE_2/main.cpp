#include <curses.h>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
	/* NCURSES START*/
	initscr();
	noecho();
	cbreak();

	/* GET SCREEN SIZE */
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	/* CREATE A WINDOW FOR OUR INPUT */
	WINDOW* menuwin = newwin(7, xMax - 12, yMax - 12, 5);
	box(menuwin, 0, 0);
	refresh();
	wrefresh(menuwin);
	/* MAKES IT SO WE CAN USE ARROW KEYS */
	keypad(menuwin, true);

	string choices[5] = { "CREATE", "RENAME", "MOVE", "COPY", "DELETE" };
	int choice;
	int highlight = 0;

	while (1)
	{
		for (int i = 0; i < 5; i++)
		{
			if (i == highlight)
				wattron(menuwin, A_REVERSE);
			mvwprintw(menuwin, i + 1, 1, choices[i].c_str());
			wattroff(menuwin, A_REVERSE);
		}
		choice = wgetch(menuwin);
		switch (choice)
		{
		case KEY_UP:
			highlight--;
			if (highlight == -1)
				highlight = 0;
			break;
		case KEY_DOWN:
			highlight++;
			if (highlight == 5)
				highlight = 4;
			break;
		default:
			break;
		}
		if (choice == 10)
			break;
	}

	printw("Your choise was : %s", choices[highlight].c_str());

	/* MAKE SURE PROGRAM WAITS BEFORE EXITING... */
	getch();
	endwin();

	/* NCURSES END */

	return 0;
}