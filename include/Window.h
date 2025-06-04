#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include "core/process.h"

struct SysStatistics {
    float CPUtotal, loadAvg[3], CPUpercore[32], memTotal = 0, memUsed = 0;
    size_t runningTasks = 0, totalTasks = 0;
    double uptimeSeconds = 0.0;
};

class Window {
public:
    virtual ~Window() = default;
    virtual void init() = 0;
    virtual void render(const std::vector<Process>& processes) = 0;
    virtual bool handleInput(size_t totalProcesses) = 0;
    virtual void shutdown() = 0;
protected:
    const int left_panel_width =35; // fixed left offset for left panel
    enum Colors {
        TITLE_COLOR,
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
};

#endif
