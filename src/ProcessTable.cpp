#include "ProcessTable.h"
#include <algorithm>

ProcessTable::ProcessTable(WINDOW* parent, CpuStatistics& cpuStats)
    : cpuStats(cpuStats)
{
    int height, width;
    getmaxyx(parent, height, width);
    window_ = derwin(parent, height, width, 0, 0);
}

ProcessTable::~ProcessTable() {
    delwin(window_);
}

void ProcessTable::init() {
    // any init
}

void ProcessTable::render(const std::vector<Process>& processes) {
    const int left_panel_width = 30;  // fixed left offset for left panel
    int max_y, max_x;
    getmaxyx(window_, max_y, max_x);

    char totalCPUUsageBuffer[16];
    wattron(window_, COLOR_PAIR(HEADER_COLOR));

    int widths[TOTAL_COLUMNS] = {8, 34, 10, 8};
    const char* headers[TOTAL_COLUMNS] = {"PID", "NAME", "CPU", "MEM%"};
    snprintf(totalCPUUsageBuffer, sizeof(totalCPUUsageBuffer), "CPU%% %.2f ", cpuStats.total);
    headers[2] = totalCPUUsageBuffer;

    int x = left_panel_width + 1;  // fixed start position, no shifting left

    for (int i = 0; i < TOTAL_COLUMNS; i++) {
        int col_width = widths[i];
        // Check if the column fits in window width
        if (x + col_width <= max_x) {
            if (i == selected_column) {
                wattron(window_, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
            } else {
                wattron(window_, COLOR_PAIR(HEADER_COLOR));
            }
            mvwprintw(window_, 1, x, "%-*s", col_width, headers[i]);
            wattroff(window_, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
            wattroff(window_, COLOR_PAIR(HEADER_COLOR));
        } else if (x < max_x) {
            // Partial display if column does not fully fit
            int available_width = max_x - x;
            if (available_width > 0) {
                char buffer[100];
                snprintf(buffer, sizeof(buffer), "%-*.*s", available_width, available_width, headers[i]);
                if (i == selected_column) {
                    wattron(window_, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
                } else {
                    wattron(window_, COLOR_PAIR(HEADER_COLOR));
                }
                mvwprintw(window_, 1, x, "%s", buffer);
                wattroff(window_, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
                wattroff(window_, COLOR_PAIR(HEADER_COLOR));
            }
            // Do not print anything if no space left
        }
        x += col_width + 2; // add spacing between columns
    }
    wattroff(window_, COLOR_PAIR(HEADER_COLOR));

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

        x = left_panel_width + 1;  // fixed starting x for each row

        // Format PID and NAME fields
        char pid_name_buffer[50];
        snprintf(pid_name_buffer, sizeof(pid_name_buffer), "%-8d %-34.34s ", p.getPid(), p.getName().c_str());
        int pid_name_len = 8 + 1 + 34 + 1; // total 44 chars including spaces

        if (x + pid_name_len <= max_x) {
            mvwprintw(window_, y, x, "%s", pid_name_buffer);
        } else if (x < max_x) {
            int avail = max_x - x;
            if (avail > 0) {
                char buf[50];
                snprintf(buf, sizeof(buf), "%.*s", avail, pid_name_buffer);
                mvwprintw(window_, y, x, "%s", buf);
            }
        }

        x += 44;

        // CPU usage field
        char cpu_buffer[10];
        snprintf(cpu_buffer, sizeof(cpu_buffer), "%5.1f ", p.getCpuUsage());
        int cpu_len = 6;

        if (x + cpu_len <= max_x) {
            wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));
            mvwprintw(window_, y, x, "%s", cpu_buffer);
            wattroff(window_, COLOR_PAIR(CPU_TEXT_COLOR));
        } else if (x < max_x) {
            int avail = max_x - x;
            if (avail > 0) {
                char buf[10];
                snprintf(buf, sizeof(buf), "%.*s", avail, cpu_buffer);
                wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));
                mvwprintw(window_, y, x, "%s", buf);
                wattroff(window_, COLOR_PAIR(CPU_TEXT_COLOR));
            }
        }

        x += cpu_len;

        // Memory usage field
        char mem_buffer[12];
        snprintf(mem_buffer, sizeof(mem_buffer), "%11.1f", p.getMemUsage());
        int mem_len = 11;

        if (x + mem_len <= max_x) {
            wattron(window_, COLOR_PAIR(MEM_TEXT_COLOR));
            mvwprintw(window_, y, x, "%s", mem_buffer);
            wattroff(window_, COLOR_PAIR(MEM_TEXT_COLOR));
        } else if (x < max_x) {
            int avail = max_x - x;
            if (avail > 0) {
                char buf[12];
                snprintf(buf, sizeof(buf), "%.*s", avail, mem_buffer);
                wattron(window_, COLOR_PAIR(MEM_TEXT_COLOR));
                mvwprintw(window_, y, x, "%s", buf);
                wattroff(window_, COLOR_PAIR(MEM_TEXT_COLOR));
            }
        }

        wattroff(window_, COLOR_PAIR(SELECTED_COLOR));
        wattroff(window_, COLOR_PAIR(PROCESS_COLOR));
    }

    wattron(window_, COLOR_PAIR(FOOTER_COLOR));
    mvwprintw(window_, max_y - 1, 0, " F1:Help  F8:Kill  F9:Quit ");
    wattroff(window_, COLOR_PAIR(FOOTER_COLOR));

    wrefresh(window_);
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
        case KEY_F(1):
            showAbout = true;
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
bool ProcessTable::getShowAbout() const {
    return showAbout;
}
void ProcessTable::setShowAbout(bool pressed) {
    showAbout = pressed;
}