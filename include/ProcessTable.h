#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include "Window.h"
#include <ncurses.h>
#include <vector>

class ProcessTable : public Window {
public:
    ProcessTable(WINDOW* parent, SysStatistics& cpuStats);
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

    bool getShowAbout() const;
    void setShowAbout(bool pressed);
private:
    size_t totalProcesses;
    WINDOW* window_;
    int selectedProcess_;
    int scrollOffset_;
    int scroll_offset;
    int selected_column;
    SysStatistics& cpuStats;
    bool showPopup = false;
    bool showAbout = false;
};

#endif
