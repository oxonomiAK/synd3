#include "LeftPanel.h"
#include <ctime>
#include "core/cpu_Stats.h"



static time_t start_time = time(nullptr);

LeftPanel::LeftPanel(WINDOW* parent, CpuStatistics& cpuStats) : cpuStats(cpuStats) {
    int height, width;
    getmaxyx(parent, height, width);
    window_ = derwin(parent, height, width, 0, 0);
}

LeftPanel::~LeftPanel() {
    delwin(window_);
}

void LeftPanel::init() {
    // Init colors or any setup if needed
}

void LeftPanel::render(const std::vector<Process>& processes) {
 int width = 30;
    size_t corecount;
    getThreadCount(&corecount, sizeof(corecount));
    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    for (int y = 1; y < LINES-1; y++) {
        mvwaddch(window_, y, width, ACS_VLINE);
    }
    wattron(window_, COLOR_PAIR(TITLE_COLOR));
    mvwprintw(window_, 1, 1, " System Information ");
    wattroff(window_, COLOR_PAIR(TITLE_COLOR));

    // CPU Usage 
    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    // mvwprintw(window_, 3, 2, "CPU Usage:");
    // wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));

    
    
    // char totalCPUUsageBuffer[16];
    // fillTotalCpuUsage(total, totalCPUUsageBuffer, sizeof(totalCPUUsageBuffer));

    // mvwprintw(window_, 3, 12,"%s%%", totalCPUUsageBuffer); // placeholder for total CPU usage

    // CPU per-core
    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(window_, 9, 1, " Per core usage: ");
    for (int i = 0; i < (int)corecount; i++) {
        float usage = cpuStats.percore[i];
        wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));
        mvwprintw(window_, 10 + i, 2, "CPU%-2d:", i);
        wattron(window_, COLOR_PAIR(CPU_BAR_COLOR));
        wprintw(window_, " [");
        int bars = (int)(usage / 10);
        for (int j = 0; j < 10; j++) {
            waddch(window_, j < bars ? ACS_CKBOARD : ' ');
        }
        wprintw(window_, "] ");
        wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));
        wprintw(window_, "%2.1f%%", usage);
    }

    // Memory Usage
    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(window_, 3, 2, "Memory Usage:");

    int panel_width = width - 4;
    int bar_width = panel_width - 12; 
    float mem_percent = 70.0f; // placeholder for memory usage percentage
    int mem_bars = (int)(bar_width * mem_percent / 100.0f);
    wattron(window_, COLOR_PAIR(MEM_BAR_COLOR));
    mvwprintw(window_, 4, 2, "[");
    for (int i = 0; i < bar_width; i++) {
        waddch(window_, i < mem_bars ? ACS_CKBOARD : ' ');
    }
    wprintw(window_, "]");

    wattron(window_, COLOR_PAIR(MEM_TEXT_COLOR));
    wprintw(window_, " %2.0f%% of 16G", mem_percent);

    // System Info
    time_t now = time(nullptr);
    int uptime = (int)difftime(now, start_time);
    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(window_, 6, 2, "Uptime: %02d:%02d:%02d",
             uptime/3600, (uptime%3600)/60, uptime%60);
    mvwprintw(window_, 7, 2, "Tasks: %ld, %d running", processes.size(), 2);
    mvwprintw(window_, 7, 2, "Load avg: 0.15, 0.30, 0.25");
    // wrefresh(window_);
}

bool LeftPanel::handleInput(size_t totalProcesses) {
    // Nothing to handle for now
    totalProcesses = totalProcesses; // Avoid unused parameter warning
    return true;
}

void LeftPanel::shutdown() {
    
}
