#pragma once

#include <curses.h>
#include <string>

#ifndef _MENU_H_
#define _MENU_H_

class Menu
{
public:
	Menu(std::string text, char trigger, std::string *items,int num_items)
	{
		this->text = text;
		this->trigger = trigger;
		this->items = items;
		this->num_items = num_items;
		this->selected_item = 0;
	}

	int start_x;
	std::string text;
	char trigger;

	std::string *items;
	int num_items;
	int selected_item;

	void selectNextItem()
	{
		selected_item++;
		if (selected_item >= num_items)
			selected_item = 0;
	}

	void selectPrevItem()
	{
		selected_item--;
		if (selected_item < 0)
			selected_item = num_items - 1;
	}

};

class MenuBar
{
public:
	MenuBar(WINDOW *win, Menu *menus, int num_menus)
	{
		this->win = win;
		this->menus = menus;
		this->num_menus = num_menus;
		this->selected_menu = -1;

		int yMax, xMax, yBeg, xBeg;
		getmaxyx(win, yMax, xMax);
		getbegyx(win, yBeg, xBeg);
		menuwin = newwin(yMax -2, xMax -2, yBeg +1, xBeg +1);
		keypad(menuwin, true);
		wrefresh(menuwin);

		int current_pos = 2;

		for (int i = 0; i < num_menus; i++)
		{
			this->menus[i].start_x = current_pos;
			current_pos += this->menus[i].text.length() + 1;
		}
	}

	void reset()
	{
		for (int i = 0; i < num_menus; i++)
		{
			int start_x = this->menus[i].start_x;
			std::string text = this->menus[i].text;
			mvwprintw(win, 0, start_x, text.c_str());
		}
		wrefresh(win);
	}

	void draw()
	{
		for (int i = 0; i < num_menus; i++)
		{
			drawMenu(menus[i], selected_menu == i);
		}
		selected_menu = -1;
	}

	void drawMenu(Menu menu, bool is_selected)
	{
		int start_x = menu.start_x;
		std::string text = menu.text;
		if (is_selected)
			wattron(win, A_STANDOUT);
		mvwprintw(win, 0, start_x, text.c_str());
		wattroff(win, A_STANDOUT);
		wrefresh(win); // Refresh okna

		int ch; // Promenna ktera kontroluje input
		drawMenuItems(menu);
		wrefresh(menuwin);
		while (is_selected && (ch = wgetch(menuwin)))
		{
			switch (ch)
			{
			case KEY_DOWN:
				menu.selectNextItem();
				break;
			case KEY_UP:
				menu.selectPrevItem();
				break;
			default:
				is_selected = false;
			}
			drawMenuItems(menu);
		}
		werase(menuwin); // Odstraneni zobrazenych polozek
		wrefresh(menuwin);
		reset();
	}

	void drawMenuItems(Menu menu)
	{
		int yMax, xMax;
		getmaxyx(menuwin, yMax, xMax);
		for (int i = 0; i < menu.num_items; i++)
		{
			mvwprintw(menuwin, i, 0, menu.items[i].c_str());
			if (menu.selected_item == i)
			{
				mvwchgat(menuwin, i, 0, xMax, A_NORMAL, 1, NULL);
			}
			else
			{
				mvwchgat(menuwin, i, 0, xMax, A_STANDOUT, 0, NULL);
			}
		}
	}

	void handleTrigger(char trigger)
	{
		for (int i = 0; i < num_menus; i++)
		{
			if (trigger == this->menus[i].trigger)
			{
				selected_menu = i;
			}
		}
	}

	WINDOW *menuwin;
	WINDOW *win;
	Menu *menus;
	int num_menus;
	int selected_menu;
};

#endif
