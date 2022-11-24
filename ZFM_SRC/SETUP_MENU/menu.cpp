#define _CRT_SECURE_NO_WARNINGS
#include <curses.h>

int main()
{
	WINDOW* w;
	char list[5][7] = { "One", "Two", "Three", "Four", "Five" };
	char item[7];
	int ch, i = 0, width = 7;
	initscr(); // inicializace ncurses
	w = newwin(10, 12, 1, 1); // nove okno
	box(w, 0, 0); // hranice okna
	// tisk vsech moznosti a oznaceni prvni moznosti
	for (i = 0; i < 5; i++) {
		if (i == 0)
			wattron(w, A_STANDOUT); // oznaceni prvni moznosti
		else
			wattroff(w, A_STANDOUT);
		sprintf(item, "%-7s", list[i]);
		mvwprintw(w, i + 1, 2, "%s", item);
	}
	wrefresh(w); // update obrazu
	i = 0;
	noecho(); // input se nezobrazuje
	keypad(w, TRUE); // povoleni inputu
	curs_set(0); // odstraneni kurzoru
	// ziskani inputu
	while ((ch = wgetch(w)) != 'q') {
		// nastaveni stejne sirky polozek
		sprintf(item, "%-7s", list[i]);
		mvwprintw(w, i + 1, 2, "%s", item);
		// snizeni nebo zvyseni hodnoty podle inputu
		switch (ch) {
		case KEY_UP:
			i--;
			i = (i < 0) ? 4 : i;
			break;
		case KEY_DOWN:
			i++;
			i = (i > 4) ? 0 : i;
			break;
		}
		// oznaceni dalsi moznosti v menu
		wattron(w, A_STANDOUT);
		sprintf(item, "%-7s", list[i]);
		mvwprintw(w, i + 1, 2, "%s", item);
		wattroff(w, A_STANDOUT);
	}
	delwin(w);
	endwin();

	return 0;
}
