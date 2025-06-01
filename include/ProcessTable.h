#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include "Window.h"
#include <ncurses.h>
#include <vector>
#include "core/process.h"

class ProcessTable : public Window {
public:
    ProcessTable(WINDOW* parent, CpuStatistics& cpuStats);
    ~ProcessTable();

    void init() override;
    void render(const std::vector<Process>& processes) override;
    bool handleInput(size_t totalProcesses) override;
    void shutdown() override;

    void setSelectedProcess(int idx);
    int getSelectedProcess() const;

    void setSelectedColumn(int col);
    int getSelectedColumn() const;

    bool getShowPopup() const;
    void setshowPopup(bool pressed);
    void resize(int height, int width, int start_y, int start_x);
private:
    size_t totalProcesses;
    WINDOW* window_;
    int selectedProcess_;
    int scrollOffset_;
    int scroll_offset;
    int selected_column;
    CpuStatistics& cpuStats;
    bool showPopup = false;
};

#endif
