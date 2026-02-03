#include "ProcessTable.h"

#include <algorithm>
#include <string.h>

ProcessTable::ProcessTable(WINDOW *parent, SysStatistics &cpuStats)
    : cpuStats(cpuStats)
{
    int height, width;
    getmaxyx(parent, height, width);
    window_ = derwin(parent, height, width, 0, 0);
}

ProcessTable::~ProcessTable()
{
    delwin(window_);
}

void ProcessTable::init()
{
    // any init
}

void ProcessTable::render(const std::vector<Process> &processes)
{
    int max_y, max_x;
    getmaxyx(window_, max_y, max_x);

    char totalCPUUsageBuffer[32], totalMEMUsageBuffer[32];
    wattron(window_, COLOR_PAIR(HEADER_COLOR));

    int widths[TOTAL_COLUMNS] = {7, 34, 10, 11};
    const char *headers[TOTAL_COLUMNS] = {"PID", "NAME", "CPU", "MEM%"};

    float percentUsed = (cpuStats.memUsed / cpuStats.memTotal) * 100.0f;
    snprintf(totalMEMUsageBuffer, sizeof(totalMEMUsageBuffer), "MEM%% %.2f", percentUsed);
    snprintf(totalCPUUsageBuffer, sizeof(totalCPUUsageBuffer), "CPU%% %.2f", cpuStats.CPUtotal);

    // Replace CPU and MEM headers with dynamic values
    headers[2] = totalCPUUsageBuffer;
    headers[3] = totalMEMUsageBuffer;

    // Update widths for CPU and MEM headers
    widths[2] = (int)strlen(totalCPUUsageBuffer);
    widths[3] = (int)strlen(totalMEMUsageBuffer);

    int x = left_panel_width + 1;

    // Render headers with updated widths
    for (int i = 0; i < TOTAL_COLUMNS; i++)
    {
        int col_width = widths[i];
        if (x + col_width <= max_x)
        {
            if (i == selected_column)
            {
                wattron(window_, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
            }
            else
            {
                wattron(window_, COLOR_PAIR(HEADER_COLOR));
            }
            mvwprintw(window_, 1, x, "%-*s", col_width, headers[i]);
            wattroff(window_, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
            wattroff(window_, COLOR_PAIR(HEADER_COLOR));
        }
        else if (x < max_x)
        {
            int available_width = max_x - x;
            if (available_width > 0)
            {
                char buffer[100];
                snprintf(buffer, sizeof(buffer), "%-*.*s", available_width, available_width, headers[i]);
                if (i == selected_column)
                {
                    wattron(window_, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
                }
                else
                {
                    wattron(window_, COLOR_PAIR(HEADER_COLOR));
                }
                mvwprintw(window_, 1, x, "%s", buffer);
                wattroff(window_, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
                wattroff(window_, COLOR_PAIR(HEADER_COLOR));
            }
        }
        x += col_width + 2;
    }
    wattroff(window_, COLOR_PAIR(HEADER_COLOR));

    int visible_lines = max_y - 3;
    int y = 2;
    int start = scroll_offset;
    int end = std::min(start + visible_lines, (int)processes.size());

    for (int i = start; i < end; i++, y++)
    {
        const auto &p = processes[i];
        if (i == selectedProcess_)
        {
            wattron(window_, COLOR_PAIR(SELECTED_COLOR));
        }
        else
        {
            wattron(window_, COLOR_PAIR(PROCESS_COLOR));
        }

        x = left_panel_width + 1;

        // Render PID and NAME as is, fixed width
        char pid_name_buffer[50];
        snprintf(pid_name_buffer, sizeof(pid_name_buffer), "%-8d %-34.34s", p.getPid(), p.getName().c_str());
        int pid_name_len = 8 + 1 + 34; // total 43 chars

        if (x + pid_name_len <= max_x)
        {
            mvwprintw(window_, y, x, "%s", pid_name_buffer);
        }
        else if (x < max_x)
        {
            int avail = max_x - x;
            if (avail > 0)
            {
                char buf[50];
                snprintf(buf, sizeof(buf), "%.*s", avail, pid_name_buffer);
                mvwprintw(window_, y, x, "%s", buf);
            }
        }

        x += pid_name_len + 1;

        // CPU usage with percent sign, right aligned
        char cpu_buffer[32];
        snprintf(cpu_buffer, sizeof(cpu_buffer), "%.2f", p.getCpuUsage());
        int cpu_width = widths[2] - 1;

        if (x + cpu_width <= max_x)
        {
            wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));
            mvwprintw(window_, y, x, "%*s", cpu_width, cpu_buffer);
            wattroff(window_, COLOR_PAIR(CPU_TEXT_COLOR));
        }
        else if (x < max_x)
        {
            int avail = max_x - x;
            if (avail > 0)
            {
                char buf[32];
                snprintf(buf, sizeof(buf), "%.*s", avail, cpu_buffer);
                wattron(window_, COLOR_PAIR(CPU_TEXT_COLOR));
                mvwprintw(window_, y, x, "%s", buf);
                wattroff(window_, COLOR_PAIR(CPU_TEXT_COLOR));
            }
        }

        x += cpu_width + 2;

        // Memory usage with percent sign, right aligned
        char mem_buffer[32];
        snprintf(mem_buffer, sizeof(mem_buffer), "%.2f", p.getMemUsage());
        int mem_width = widths[3];

        if (x + mem_width <= max_x)
        {
            wattron(window_, COLOR_PAIR(MEM_TEXT_COLOR));
            mvwprintw(window_, y, x, "%*s", mem_width, mem_buffer);
            wattroff(window_, COLOR_PAIR(MEM_TEXT_COLOR));
        }
        else if (x < max_x)
        {
            int avail = max_x - x;
            if (avail > 0)
            {
                char buf[32];
                snprintf(buf, sizeof(buf), "%.*s", avail, mem_buffer);
                wattron(window_, COLOR_PAIR(MEM_TEXT_COLOR));
                mvwprintw(window_, y, x, "%s", buf);
                wattroff(window_, COLOR_PAIR(MEM_TEXT_COLOR));
            }
        }

        wattroff(window_, COLOR_PAIR(SELECTED_COLOR));
        wattroff(window_, COLOR_PAIR(PROCESS_COLOR));
    }

    // Render footer as is
    wattron(window_, COLOR_PAIR(FOOTER_COLOR));
    const char *footer = " F1:Help  F9:Kill  F10:Quit ";
    int footer_len = static_cast<int>(strlen(footer));
    mvwprintw(window_, max_y - 1, max_x - footer_len, "%s", footer);
    wattroff(window_, COLOR_PAIR(FOOTER_COLOR));

    wrefresh(window_);
}

bool ProcessTable::handleInput(size_t totalProcesses)
{
    int ch = getch();
    MEVENT event;
    switch (ch)
    {
    case KEY_UP:
        if (selectedProcess_ > 0)
            selectedProcess_--;
        break;
    case KEY_DOWN:
        if (selectedProcess_ < (int)totalProcesses - 1)
            selectedProcess_++;
        break;
    case KEY_LEFT:
        selected_column = (selected_column - 1 + TOTAL_COLUMNS) % TOTAL_COLUMNS;
        break;
    case KEY_RIGHT:
        selected_column = (selected_column + 1) % TOTAL_COLUMNS;
        break;
    case KEY_F(10):
        return false;

    case KEY_F(9):
        showPopup = true;
        break;
    case KEY_F(1):
        showAbout = true;
        break;
    case KEY_MOUSE:
        if (getmouse(&event) == OK)
        {
            if (event.bstate & BUTTON4_PRESSED)
            {
                if (selectedProcess_ > 0)
                    selectedProcess_--;
            }
            else if (event.bstate & BUTTON5_PRESSED)
            {
                if (selectedProcess_ < (int)totalProcesses - 1)
                    selectedProcess_++;
            }
        }
        break;
    }
    int max_y, max_x;
    getmaxyx(window_, max_y, max_x);
    int visible_lines = max_y - 3;
    if (selectedProcess_ < scroll_offset)
    {
        scroll_offset = selectedProcess_;
    }
    else if (selectedProcess_ >= scroll_offset + visible_lines)
    {
        scroll_offset = selectedProcess_ - visible_lines + 1;
    }
    return true;
}

void ProcessTable::shutdown()
{
    // cleanup
}
void ProcessTable::setSelectedColumn(int col)
{
    selected_column = col;
}
int ProcessTable::getSelectedColumn() const
{
    return selected_column;
};

void ProcessTable::setSelectedProcess(int idx)
{
    selectedProcess_ = idx;
}

int ProcessTable::getSelectedProcess() const
{
    return selectedProcess_;
}

bool ProcessTable::getShowPopup() const
{
    return showPopup;
}
void ProcessTable::setshowPopup(bool pressed)
{
    showPopup = pressed;
}
bool ProcessTable::getShowAbout() const
{
    return showAbout;
}
void ProcessTable::setShowAbout(bool pressed)
{
    showAbout = pressed;
}