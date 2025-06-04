#include "LeftPanel.h"
#include <ctime>
#include <cstring>
#include "core/cpu_Stats.h"
#include <sstream>

static time_t start_time = time(nullptr);

LeftPanel::LeftPanel(WINDOW* parent, SysStatistics& cpuStats) : cpuStats(cpuStats) {
    int height, width;
    getmaxyx(parent, height, width);
    window_ = derwin(parent, height, width, 0, 0);
    getCpuName(cpuName, sizeof(cpuName));
}

LeftPanel::~LeftPanel() {
    delwin(window_);
}

void LeftPanel::init() {
    // Init colors or any setup if needed
}

void LeftPanel::render(const std::vector<Process>& processes) {
    int max_y, max_x;
    getmaxyx(window_, max_y, max_x);

    size_t corecount;
    getThreadCount(&corecount, sizeof(corecount));

    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    // Draw vertical separator line at left_panel_width
    for (int y = 1; y < max_y - 1; y++) {
        mvwaddch(window_, y, left_panel_width, ACS_VLINE);
    }

    int line = 1;
    bool first_line = true;
    // Title
    wattron(window_, COLOR_PAIR(TITLE_COLOR));
    mvwprintw(window_, line++, 1, " System Information ");
    wattroff(window_, COLOR_PAIR(TITLE_COLOR));
    line++;
    // CPU name (wrap to next line if too long)
    wattron(window_, COLOR_PAIR(TEXT_COLOR));

    int name_max_width = left_panel_width - 7;

    std::istringstream iss(cpuName);
    std::string word;
    std::string lineBuffer;

    while (iss >> word) {
        if ((int)(lineBuffer.size() + word.size() + (lineBuffer.empty() ? 0 : 1)) > name_max_width) {
            // if the line exceeds the max width, print it
            if (!lineBuffer.empty()) {
                if (first_line) {
                    mvwprintw(window_, line++, 2, "CPU: %s", lineBuffer.c_str());
                    first_line = false;
                } else {
                    mvwprintw(window_, line++, 7, "%s", lineBuffer.c_str());
                }
                lineBuffer.clear();
            }
        }

        if (!lineBuffer.empty()) lineBuffer += " ";
        lineBuffer += word;
    }

    // print any remaining text in lineBuffer
    if (!lineBuffer.empty()) {
        if (first_line) {
            mvwprintw(window_, line++, 2, "CPU: %s", lineBuffer.c_str());
        } else {
            mvwprintw(window_, line++, 7, "%s", lineBuffer.c_str());
        }
    }

    line++; // add extra line after CPU name
    // Memory Usage
    mvwprintw(window_, line++, 2, "Memory Usage:");
    int mem_bar_width = left_panel_width - 4 - 16;
    
    float memUsedInGB = cpuStats.memUsed / 1024.0f / 1024.0f;
    float memTotalInGB = cpuStats.memTotal / 1024.0f / 1024.0f;
    float memUsageRatio = memUsedInGB / memTotalInGB;
    
    int mem_bars = static_cast<int>(mem_bar_width * memUsageRatio);
    
    // Draw memory usage bar
    wattron(window_, COLOR_PAIR(MEM_BAR_COLOR));
    mvwprintw(window_, line, 2, "[");
    for (int i = 0; i < mem_bar_width; i++) {
        waddch(window_, i < mem_bars ? ACS_CKBOARD : ' ');
    }
    wprintw(window_, "]");
    
    // Draw memory usage text (MB if <1GB, else GB)
    wattron(window_, COLOR_PAIR(MEM_TEXT_COLOR));
    if (memUsedInGB < 1.0f) {
        wprintw(window_, " %.0fM of %.1fG", cpuStats.memUsed / 1024.0f, memTotalInGB);
    } else {
        wprintw(window_, " %.2fG of %.1fG", memUsedInGB, memTotalInGB);
    }
    line += 2;

    // Per-core CPU usage
    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(window_, line++, 1, " Per core usage: ");

    int system_info_lines = 5;
    int available_lines_for_cpu = max_y - line - system_info_lines;
    int width_for_cpu = left_panel_width - 18;
    int cores_to_show = std::min(static_cast<int>(corecount), available_lines_for_cpu);

    for (int i = 0; i < cores_to_show; i++) {
        float usage = cpuStats.CPUpercore[i];
        wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));
        mvwprintw(window_, line, 2, "CPU%-2d:", i);
        wattron(window_, COLOR_PAIR(CPU_BAR_COLOR));
        wprintw(window_, " [");
        int bars = static_cast<int>(usage * width_for_cpu / 100.0f);
        for (int j = 0; j < width_for_cpu; j++) {
            waddch(window_, j < bars ? ACS_CKBOARD : ' ');
        }
        wprintw(window_, "] ");
        wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));
        wprintw(window_, "%2.1f%%", usage);
        line++;
    }
    
    // System info at the bottom
   
    int total_seconds = static_cast<int>(cpuStats.uptimeSeconds);

    int days = total_seconds / 86400;
    int hours = (total_seconds % 86400) / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;
    
    wattron(window_, COLOR_PAIR(TEXT_COLOR));
    mvwprintw(window_, max_y - 5, 2, "Uptime: %d days %02d:%02d:%02d",
          days, hours, minutes, seconds);
    mvwprintw(window_, max_y - 4, 2, "Tasks: %ld, %ld running", processes.size(), cpuStats.runningTasks);
    mvwprintw(window_, max_y - 3, 2, "Load avg: %.2f %.2f %.2f",
              cpuStats.loadAvg[0], cpuStats.loadAvg[1], cpuStats.loadAvg[2]);
}

bool LeftPanel::handleInput(size_t /*totalProcesses*/) {
    return true;
}

void LeftPanel::shutdown() {
    // No shutdown logic for now
}
