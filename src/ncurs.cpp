#include "ncurs.h"
#include <ncurses.h>
#include <algorithm>
#include <vector>
#include <string>
#include <unistd.h>
#include <ctime>
#include "core/process.h"
#include <chrono>
#include <thread>

NcursesUI::NcursesUI() {}

enum Colors {
    TITLE_COLOR = 1,
    HEADER_COLOR,
    SELECTED_COLOR,
    PROCESS_COLOR,
    CPU_BAR_COLOR,
    MEM_BAR_COLOR,
    TEXT_COLOR,
    HIGHLIGHT_COLOR,
    CPU_TEXT_COLOR,
    MEM_TEXT_COLOR,
    FOOTER_COLOR
};


enum Columns {
    COL_PID,
    COL_NAME,
    COL_CPU,
    COL_MEM,
    TOTAL_COLUMNS
};

static int selected_process = 0;
static int selected_column = COL_CPU;
static int scroll_offset = 0;
static time_t start_time = time(nullptr);
static WINDOW* main_win = nullptr;

void NcursesUI::init() {
    initscr();
    cbreak();
    noecho(); 
    curs_set(0);
    start_color();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    refresh();

    main_win = newwin(LINES, COLS, 0, 0);
    keypad(main_win, TRUE);

    init_pair(TITLE_COLOR, COLOR_BLACK, COLOR_CYAN);
    init_pair(HEADER_COLOR, COLOR_BLACK, COLOR_GREEN);
    init_pair(SELECTED_COLOR, COLOR_WHITE, COLOR_BLUE);
    init_pair(PROCESS_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(CPU_BAR_COLOR, COLOR_BLACK, COLOR_GREEN);
    init_pair(MEM_BAR_COLOR, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(TEXT_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(HIGHLIGHT_COLOR, COLOR_YELLOW, COLOR_BLUE);
    init_pair(CPU_TEXT_COLOR, COLOR_CYAN, COLOR_BLACK);
    init_pair(MEM_TEXT_COLOR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(FOOTER_COLOR, COLOR_CYAN, COLOR_BLACK); 
}

void refresh_screen() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    refresh();
}

void NcursesUI::shutdown() {
    delwin(main_win);
    endwin();
}

static void draw_left_panel(int process_count) {
    int width = 30;
    wattron(main_win, COLOR_PAIR(TEXT_COLOR));
    for (int y = 1; y < LINES-1; y++) {
        mvwaddch(main_win, y, width, ACS_VLINE);
    }
    wattron(main_win, COLOR_PAIR(TITLE_COLOR));
    mvwprintw(main_win, 1, 1, " System Information ");
    wattroff(main_win, COLOR_PAIR(TITLE_COLOR));

    // CPU Usage 
    wattron(main_win, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(main_win, 3, 2, "CPU Usage:");
    wattron(main_win, COLOR_PAIR(CPU_TEXT_COLOR));
    mvwprintw(main_win, 3, 13, "53%%"); // placeholder for total CPU usage

    // CPU per-core
    wattron(main_win, COLOR_PAIR(TEXT_COLOR));
    for (int i = 0; i < 12; i++) {
        float usage = 10 + (rand() % 60);
        wattron(main_win, COLOR_PAIR(CPU_TEXT_COLOR));
        mvwprintw(main_win, 4 + i, 2, "CPU%-2d:", i);
        wattron(main_win, COLOR_PAIR(CPU_BAR_COLOR));
        wprintw(main_win, " [");
        int bars = (int)(usage / 10);
        for (int j = 0; j < 10; j++) {
            waddch(main_win, j < bars ? ACS_CKBOARD : ' ');
        }
        wprintw(main_win, "] ");
        wattron(main_win, COLOR_PAIR(CPU_TEXT_COLOR));
        wprintw(main_win, "%2.0f%%", usage);
    }

    // Memory Usage
    wattron(main_win, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(main_win, 17, 2, "Memory Usage:");

    int panel_width = width - 4;
    int bar_width = panel_width - 12; 
    float mem_percent = 70.0f; // placeholder for memory usage percentage
    int mem_bars = (int)(bar_width * mem_percent / 100.0f);
    wattron(main_win, COLOR_PAIR(MEM_BAR_COLOR));
    mvwprintw(main_win, 18, 2, "[");
    for (int i = 0; i < bar_width; i++) {
        waddch(main_win, i < mem_bars ? ACS_CKBOARD : ' ');
    }
    wprintw(main_win, "]");

    wattron(main_win, COLOR_PAIR(MEM_TEXT_COLOR));
    wprintw(main_win, " %2.0f%% of 16G", mem_percent);

    // System Info
    time_t now = time(nullptr);
    int uptime = (int)difftime(now, start_time);
    wattron(main_win, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(main_win, 20, 2, "Uptime: %02d:%02d:%02d",
             uptime/3600, (uptime%3600)/60, uptime%60);
    mvwprintw(main_win, 21, 2, "Tasks: %d, %d running", process_count, 2);
    mvwprintw(main_win, 22, 2, "Load avg: 0.15, 0.30, 0.25");
}

static void draw_header() {
    int width = 30;
    wattron(main_win, COLOR_PAIR(HEADER_COLOR));
    int widths[TOTAL_COLUMNS] = {8, 34, 8, 8};
    const char* headers[TOTAL_COLUMNS] = {"PID", "NAME", "CPU%", "MEM%"};
    int x = width + 1;
    for (int i = 0; i < TOTAL_COLUMNS; i++) {
        if (i == selected_column) {
            wattron(main_win, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
        } else {
            wattron(main_win, COLOR_PAIR(HEADER_COLOR));
        }
        mvwprintw(main_win, 1, x, "%-*s", widths[i], headers[i]);
        x += widths[i] + 2;
    }
    wattroff(main_win, COLOR_PAIR(HEADER_COLOR));
}

void NcursesUI::render(const std::vector<Process>& processes) {
    werase(main_win);
    box(main_win, 0, 0);
    draw_left_panel(processes.size());
    draw_header();

    int width = 30;
    int max_y, max_x;
    getmaxyx(main_win, max_y, max_x);
    int visible_lines = max_y - 3;
    int start = scroll_offset;
    int end = std::min(start + visible_lines, (int)processes.size());

    int y = 2;
    for (int i = start; i < end; i++, y++) {
        const auto& p = processes[i];
        if (i == selected_process) {
            wattron(main_win, COLOR_PAIR(SELECTED_COLOR));
        } else {
            wattron(main_win, COLOR_PAIR(PROCESS_COLOR));
        }
        mvwprintw(main_win, y, width + 1, "%-8d %-34.34s ", p.getPid(), p.getName().c_str());
        wattron(main_win, COLOR_PAIR(CPU_TEXT_COLOR));
        wprintw(main_win, "%6.1f ", p.getCpuUsage());
        wattron(main_win, COLOR_PAIR(MEM_TEXT_COLOR));
        wprintw(main_win, "%6.1f", p.getMemUsage());
        wattroff(main_win, COLOR_PAIR(SELECTED_COLOR));
        wattroff(main_win, COLOR_PAIR(PROCESS_COLOR));
    }

    // ФУТЕР — теперь светло-голубой
    wattron(main_win, COLOR_PAIR(FOOTER_COLOR));
    mvwprintw(main_win, LINES - 1, 0, " F1:Help  F2:Setup  F3:Search  F4:Filter  F5:Tree  F6:SortBy  F7:Nice  F8:Kill  F9:Quit ");
    wattroff(main_win, COLOR_PAIR(FOOTER_COLOR));

    wrefresh(main_win);
}

bool NcursesUI::handleInput(size_t totalProcesses) {
    int ch = wgetch(main_win);
    switch(ch) {
        case KEY_UP:
            if (selected_process > 0) selected_process--;
            break;
        case KEY_DOWN:
            if (selected_process < (int)totalProcesses - 1) selected_process++;
            break;
        case KEY_LEFT:
            selected_column = (selected_column - 1 + TOTAL_COLUMNS) % TOTAL_COLUMNS;
            break;
        case KEY_RIGHT:
            selected_column = (selected_column + 1) % TOTAL_COLUMNS;
            break;
        case KEY_PPAGE:
            selected_process = std::max(0, selected_process - 10);
            break;
        case KEY_NPAGE:
            selected_process = std::min((int)totalProcesses - 1, selected_process + 10);
            break;
        case 'q':
        case KEY_F(9):
            return false;
        case KEY_F(6):
            selected_column = (selected_column + 1) % TOTAL_COLUMNS;
            break;
    }
    int max_y, max_x;
    getmaxyx(main_win, max_y, max_x);
    int visible_lines = max_y - 3;
    if (selected_process < scroll_offset) {
        scroll_offset = selected_process;
    } else if (selected_process >= scroll_offset + visible_lines) {
        scroll_offset = selected_process - visible_lines + 1;
    }
    return true;
}