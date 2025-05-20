#include <ncurses.h>
#include "../include/ncurs.h"

void NcursesUI::init()
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

void NcursesUI::render(const std::vector<Process> &processes)
{
    clear();
    mvprintw(0, 0, "PID     NAME                                 CPU%%   MEM%%");
    for (size_t i = 0; i < processes.size(); ++i)
    {
        const auto &p = processes[i];
        mvprintw(i + 1, 0, "%5d   %-34s %5.1f   %5.1f",
                 p.getPid(), p.getName().c_str(), p.getCpuUsage(), p.getMemUsage());
    }
    refresh();
}

void NcursesUI::shutdown()
{
    endwin();
}