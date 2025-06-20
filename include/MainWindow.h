#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "LeftPanel.h"
#include "ProcessTable.h"
#include "PopupWindow.h"
#include "AboutWindow.h"
#include <vector>
#include "core/process.h"
#include <ncurses.h>
#include "core/cpu_Stats.h"
#include "core/mem_Info.h"


class MainWindow : public Window {
public:
    MainWindow(std::vector<Process> &processes);
    ~MainWindow();

    void init() override;
    void render(const std::vector<Process>& processes) override;
    bool handleInput(size_t totalProcesses) override;
    void shutdown() override;

    int getSelectedColumn() const;
    void setSelectedColumn(int col);
    bool getShowPopupWindow() const;
    bool getShowAboutWindow() const;
    SysStatistics& getSysStatisics();

private:
    void showAbout(size_t totalProcesses);
    void showPopup(size_t totalProcesses);

    WINDOW* mainWin_ = nullptr;
    LeftPanel* leftPanel_ = nullptr;
    ProcessTable* processTable_ = nullptr;
    PopupWindow* popupWindow_ = nullptr;
    AboutWindow* aboutWindow_ = nullptr;

    std::vector<Window*> windows_;

    bool showPopupWindow_ = false;
    bool showOverlayWindow_ = false;
    bool showAboutWindow_ = false;
    std::vector<Process> &processes;       
    bool running = true;
    SysStatistics SysStat;
};

#endif
