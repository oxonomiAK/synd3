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
    const int panel_width = 30;  // fixed width for left panel
    int max_y, max_x;
    getmaxyx(window_, max_y, max_x);

    size_t corecount;
    getThreadCount(&corecount, sizeof(corecount));

    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    // Draw vertical separator line at panel_width
    for (int y = 1; y < max_y - 1; y++) {
        mvwaddch(window_, y, panel_width, ACS_VLINE);
    }

    wattron(window_, COLOR_PAIR(TITLE_COLOR));
    mvwprintw(window_, 1, 1, " System Information ");
    wattroff(window_, COLOR_PAIR(TITLE_COLOR));

    // Memory Usage bar at fixed position near the top
    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(window_, 3, 2, "Memory Usage:");

    int mem_bar_width = panel_width - 4 - 12; // 12 for labels and spacing
    float mem_percent = 70.0f; // example placeholder value
    int mem_bars = (int)(mem_bar_width * mem_percent / 100.0f);
    wattron(window_, COLOR_PAIR(MEM_BAR_COLOR));
    mvwprintw(window_, 4, 2, "[");
    for (int i = 0; i < mem_bar_width; i++) {
        waddch(window_, i < mem_bars ? ACS_CKBOARD : ' ');
    }
    wprintw(window_, "]");
    wattron(window_, COLOR_PAIR(MEM_TEXT_COLOR));
    wprintw(window_, " %2.0f%% of 16G", mem_percent);

    // CPU per-core usage - clipped if not enough space at bottom
    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(window_, 6, 1, " Per core usage: ");

    // Calculate how many lines are available for core bars without overlapping bottom system info
    int system_info_lines = 5;  // number of lines reserved at bottom for uptime etc.
    int available_lines_for_cpu = max_y - 7 - system_info_lines; 
    int cores_to_show = std::min((int)corecount, available_lines_for_cpu);

    for (int i = 0; i < cores_to_show; i++) {
        float usage = cpuStats.percore[i];
        wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));
        mvwprintw(window_, 7 + i, 2, "CPU%-2d:", i);
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

    // System information at the bottom (fixed position)
    time_t now = time(nullptr);
    int uptime = (int)difftime(now, start_time);
    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(window_, max_y - 5, 2, "Uptime: %02d:%02d:%02d",
             uptime / 3600, (uptime % 3600) / 60, uptime % 60);
    mvwprintw(window_, max_y - 4, 2, "Tasks: %ld, %d running", processes.size(), 2);
    mvwprintw(window_, max_y - 3, 2, "Load avg: 0.15, 0.30, 0.25");
}

bool LeftPanel::handleInput(size_t /*totalProcesses*/) {
    // Nothing to handle for now

    return true;
}

void LeftPanel::shutdown() {
    
}
