#ifndef LEFTPANEL_H
#define LEFTPANEL_H

#include "Window.h"
#include <ncurses.h>
#include <vector>
#include "core/process.h"


class LeftPanel : public Window {
public:
    LeftPanel(WINDOW* parent, CpuStatistics& cpuStats);
    ~LeftPanel();

    void init() override;
    void render(const std::vector<Process>& processes) override;
    bool handleInput(size_t totalProcesses) override;
    void shutdown() override;

private:
    WINDOW* window_;
    CpuStatistics& cpuStats;
};

#endif
