#include "tui/main_window.h"

void initMainWindow(WINDOW **main_window)
{
    *main_window = newwin(getmaxy(stdscr), getmaxx(stdscr), 0, 0);
}



void drawMainWindow(WINDOW *main_window)
{
    werase(main_window);
    box(main_window, 0, 0);
    int maxx, maxy;
    getmaxyx(main_window, maxy, maxx);


    wattron(main_window, COLOR_PAIR(FOOTER_COLOR));
    const char *footer = " F1:Help  F9:Kill  F10:Quit ";
    int footerLen = (int)(strlen(footer));
    mvwprintw(main_window, maxy - 1, maxx - footerLen, "%s", footer);
    wattroff(main_window, COLOR_PAIR(FOOTER_COLOR));

    wnoutrefresh(main_window);
}