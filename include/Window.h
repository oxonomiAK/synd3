#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include "core/process.h"

struct CpuStatistics {
    float total, loadAvg[3], percore[32];
    size_t runningTasks = 0;
};

class Window {
public:
    virtual ~Window() = default;
    virtual void init() = 0;
    virtual void render(const std::vector<Process>& processes) = 0;
    virtual bool handleInput(size_t totalProcesses) = 0;
    virtual void shutdown() = 0;
protected:
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
