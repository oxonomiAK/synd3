#include <ncurses.h>
#include "../include/ncurs.h"
#include "../include/lib.h"

void init_ui()
{
    initscr();
    printw("Interface of glance was started!\n");
    refresh();
    print_msg();
    getch();
    endwin();
}