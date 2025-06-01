#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "LeftPanel.h"
#include "ProcessTable.h"
#include "PopupWindow.h"
#include <vector>
#include "core/process.h"
#include <ncurses.h>
#include <thread>
#include <atomic>
#include <mutex>
#include "core/cpu_Stats.h"



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
private:
    void showPopup(size_t totalProcesses);
    LeftPanel* leftPanel_;
    ProcessTable* processTable_;
    WINDOW* mainWin_;
    PopupWindow* popupWindow_;
    bool showPopupWindow_;
    bool showOverlayWindow_;
    std::vector<Process> &processes;
    std::thread updateStatsThread;
    void updateStatsLoop();         
    std::atomic<bool> running;
    std::mutex mtx;
    // int sortColumn_;
    CpuStatistics cpu;

};

#endif
