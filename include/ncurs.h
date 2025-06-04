#ifndef NCURS_H
#define NCURS_H

#include <vector>
#include "core/process.h"

class NcursesUI
{
public:
    NcursesUI();
    void init();
    void render(const std::vector<Process> &processes);
    void shutdown();
    bool handleInput(size_t totalProcesses);
    int getSortColumn() const { return sort_column_; }
    void setSortColumn(int column) { sort_column_ = column; }
    

private:
    std::thread updateStatsThread;
    void updateStatsLoop();         
    size_t selected_process_;
    size_t scroll_offset_;
    int sort_column_;
    std::atomic<bool> running;
};
#endif