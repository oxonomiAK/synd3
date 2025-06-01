#include "ProcessTable.h"
#include <algorithm>

ProcessTable::ProcessTable(WINDOW* parent, CpuStatistics& cpuStats)
    : cpuStats(cpuStats)
{
    int height, width;
    getmaxyx(parent, height, width);
    // Создаем отдельное подокно справа от левой панели шириной 30
    window_ = derwin(parent, height, width, 0, 0);
}

ProcessTable::~ProcessTable() {
    delwin(window_);
}

void ProcessTable::init() {
    // any init
}

void ProcessTable::render(const std::vector<Process>& processes) {
    int width = 30;
    char totalCPUUsageBuffer[16];
    wattron(window_, COLOR_PAIR(HEADER_COLOR));

    int widths[TOTAL_COLUMNS] = {8, 34, 9, 8};
    
    const char* headers[TOTAL_COLUMNS] = {"PID", "NAME", "CPU", "MEM%"};
    snprintf(totalCPUUsageBuffer, sizeof(totalCPUUsageBuffer), "CPU%% %.2f ", cpuStats.total);
    headers[2] = totalCPUUsageBuffer;
    int x = width + 1;
    for (int i = 0; i < TOTAL_COLUMNS; i++) {
        if (i == selected_column) {
            wattron(window_, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
        } else {
            wattron(window_, COLOR_PAIR(HEADER_COLOR));
        }
        mvwprintw(window_, 1, x, "%-*s", widths[i], headers[i]);
        x += widths[i] + 2;
    }
    wattroff(window_, COLOR_PAIR(HEADER_COLOR));
    int max_y, max_x;
    getmaxyx(window_, max_y, max_x);
    int visible_lines = max_y - 3;
    int y = 2;
    int start = scroll_offset;
    int end = std::min(start + visible_lines, (int)processes.size());
    for (int i = start; i < end; i++, y++) {
        const auto& p = processes[i];
        if (i == selectedProcess_) {
            wattron(window_, COLOR_PAIR(SELECTED_COLOR));
        } else {
            wattron(window_, COLOR_PAIR(PROCESS_COLOR));
        }
        mvwprintw(window_, y, width + 1, "%-8d %-34.34s ", p.getPid(), p.getName().c_str());
        wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));
        wprintw(window_, "%5.1f ", p.getCpuUsage());
        wattron(window_, COLOR_PAIR(MEM_TEXT_COLOR));
        wprintw(window_, "%10.1f", p.getMemUsage());
        wattroff(window_, COLOR_PAIR(SELECTED_COLOR));
        wattroff(window_, COLOR_PAIR(PROCESS_COLOR));
    }


    wattron(window_, COLOR_PAIR(FOOTER_COLOR));
    mvwprintw(window_, LINES - 1, 0, " F1:Help  F2:Setup  F3:Search  F4:Filter  F5:Tree  F6:SortBy  F7:Nice  F8:Kill  F9:Quit ");
    wattroff(window_, COLOR_PAIR(FOOTER_COLOR));

    // wrefresh(window_);
}

bool ProcessTable::handleInput(size_t totalProcesses) {
int ch = getch();
    MEVENT event;
    switch(ch) {
        case KEY_UP:
            if (selectedProcess_ > 0) selectedProcess_--;
            break;
        case KEY_DOWN:
            if (selectedProcess_ < (int)totalProcesses - 1) selectedProcess_++;
            break;
        case KEY_LEFT:
            selected_column = (selected_column - 1 + TOTAL_COLUMNS) % TOTAL_COLUMNS;
            break;
        case KEY_RIGHT:
            selected_column = (selected_column + 1) % TOTAL_COLUMNS;
            break;
        case KEY_PPAGE:
            selectedProcess_ = std::max(0, selectedProcess_ - 10);
            break;
        case KEY_NPAGE:
            selectedProcess_ = std::min((int)totalProcesses - 1, selectedProcess_ + 10);
            break;
        case 'q':
            return false;
        case KEY_F(9):
            return false;
        case KEY_F(8):
            showPopup = true; 
            break;
        case KEY_F(6):
            selected_column = (selected_column + 1) % TOTAL_COLUMNS;
            break;
        case KEY_MOUSE:
            if (getmouse(&event) == OK) {
                if (event.bstate & BUTTON4_PRESSED) {
                    if (selectedProcess_ > 0) selectedProcess_--;
                } else if (event.bstate & BUTTON5_PRESSED) {
                    if (selectedProcess_ < (int)totalProcesses - 1) selectedProcess_++;
                }
            }
            break;
    }
    int max_y, max_x;
    getmaxyx(window_, max_y, max_x);
    int visible_lines = max_y - 3;
    if (selectedProcess_ < scroll_offset) {
        scroll_offset = selectedProcess_;
    } else if (selectedProcess_ >= scroll_offset + visible_lines) {
        scroll_offset = selectedProcess_ - visible_lines + 1;
    }
    return true;
}

void ProcessTable::shutdown() {
    // cleanup
}
void ProcessTable::setSelectedColumn(int col) {
    selected_column = col;
}
int ProcessTable::getSelectedColumn() const{
    return selected_column;
};

void ProcessTable::setSelectedProcess(int idx) {
    selectedProcess_ = idx;
}

int ProcessTable::getSelectedProcess() const {
    return selectedProcess_;
}

bool ProcessTable::getShowPopup() const {
    return showPopup;
}
void ProcessTable::setshowPopup(bool pressed) {
    showPopup = pressed;
}