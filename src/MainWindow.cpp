#include "MainWindow.h"
#include <ncurses.h>



MainWindow::MainWindow(std::vector<Process> &processes)
    : mainWin_(nullptr),
      leftPanel_(nullptr),
      processTable_(nullptr),
      popupWindow_(nullptr),
      aboutWindow_(nullptr),
      windows_(),
      showPopupWindow_(false),
      showOverlayWindow_(false),
      showAboutWindow_(false),
      processes(processes),
      updateStatsThread(),
      running(false),
      mtx(),
      cpu()
{
    cpu.total = 0.0f;
    for (int i = 0; i < 32; ++i) {
        cpu.percore[i] = 0.0f;
    }
}


MainWindow::~MainWindow() {
    shutdown();
    running = false;
    if (updateStatsThread.joinable()) {
        updateStatsThread.join();
    }
}

void MainWindow::init() {
    int height, width;
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    start_color();
    refresh();
    getmaxyx(stdscr, height, width);
    mainWin_ = newwin(height, width, 0, 0);
    keypad(mainWin_, TRUE);
    nodelay(mainWin_, TRUE);


    leftPanel_ = new LeftPanel(mainWin_, cpu);
    processTable_ = new ProcessTable(mainWin_, cpu);
    popupWindow_ = new PopupWindow(mainWin_, 10, 60, -1, -1, "", processes, true);
    aboutWindow_ = new AboutWindow(mainWin_);


    windows_.push_back(leftPanel_);
    windows_.push_back(processTable_);
    windows_.push_back(popupWindow_);
    windows_.push_back(aboutWindow_);

    for (auto* win : windows_) win->init();
    showPopupWindow_ = false;
    showAboutWindow_ = false;

    // Initialize color pairs here or in main
    init_pair(TITLE_COLOR, 16, COLOR_CYAN);
    init_pair(HEADER_COLOR, 144, 17);
    init_pair(SELECTED_COLOR, COLOR_WHITE, COLOR_BLUE);
    init_pair(PROCESS_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(CPU_BAR_COLOR, COLOR_BLACK, COLOR_GREEN);
    init_pair(MEM_BAR_COLOR, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(TEXT_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(HIGHLIGHT_COLOR, 15, COLOR_BLUE);
    init_pair(CPU_TEXT_COLOR, COLOR_CYAN, COLOR_BLACK);
    init_pair(MEM_TEXT_COLOR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(FOOTER_COLOR, COLOR_CYAN, COLOR_BLACK); 
    running = true;
    updateStatsThread = std::thread(&MainWindow::updateStatsLoop, this);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    wrefresh(mainWin_);
}
void MainWindow::updateStatsLoop() {
    std::lock_guard<std::mutex> lock(mtx);
    while (running) {
        getCpuStats(&cpu.total, cpu.percore, sizeof(cpu.total), sizeof(cpu.percore));    
        getLoadavgData(cpu.loadAvg, &cpu.runningTasks, &cpu.totalTasks, sizeof(cpu.loadAvg), sizeof(cpu.runningTasks), sizeof(cpu.totalTasks));
        getUptimeSeconds(&cpu.uptimeSeconds, sizeof(cpu.uptimeSeconds));
        cpu.totalTasks++; // Increment total tasks to match the original logic
    }
}


void MainWindow::render(const std::vector<Process>& processes) {
    werase(mainWin_);
    box(mainWin_, 0, 0);




    for (auto* win : windows_) {

        if (win == popupWindow_ && !showPopupWindow_) continue;
        if (win == aboutWindow_ && !showAboutWindow_) continue;
        win->render(processes);
    }



    // // Overlay window placeholder
    if (showOverlayWindow_) {
        WINDOW* overlay = newwin(5, 50, 2, 20);
        wattron(overlay, A_BOLD);
        box(overlay, 0, 0);
        mvwprintw(overlay, 1, 2, "Overlay window placeholder");
        mvwprintw(overlay, 3, 2, "Always on top");
        wattroff(overlay, A_BOLD);
        wrefresh(overlay);
        delwin(overlay);
    }

    wrefresh(mainWin_);
}
void MainWindow::showPopup(size_t totalProcesses) {
    popupWindow_->show("Are you sure you want to kill this process?");
    popupWindow_->setSelectedProcess(processTable_->getSelectedProcess());
    popupWindow_->render(processes);
    
    showPopupWindow_ = popupWindow_->handleInput(totalProcesses);
    processTable_->setshowPopup(showPopupWindow_);

}

void MainWindow::showAbout(size_t totalProcesses) {
    aboutWindow_->show();
    aboutWindow_->render(processes);

    
    showAboutWindow_ = aboutWindow_->handleInput(totalProcesses);
    processTable_->setShowAbout(showPopupWindow_);
}

bool MainWindow::handleInput(size_t totalProcesses) {
    running.store(processTable_->handleInput(totalProcesses));
    showPopupWindow_ = processTable_->getShowPopup();
    showAboutWindow_ = processTable_->getShowAbout();
   
    if (showPopupWindow_) {
        showPopup(totalProcesses);
    }

    if(showAboutWindow_) {
        showAbout(totalProcesses);
    }

    return running.load();
}

int MainWindow::getSelectedColumn() const {
    return processTable_->getSelectedColumn();
}
void MainWindow::setSelectedColumn(int col) {
    processTable_->setSelectedColumn(col);
}

void MainWindow::shutdown() {
    for (auto* win : windows_) {
        if (win) {
            win->shutdown();
        }
    }
    for (auto* win : windows_) {
        delete win;
    }
    windows_.clear();
    if (mainWin_) {
        delwin(mainWin_);
        mainWin_ = nullptr;
    }
    leftPanel_ = nullptr;
    processTable_ = nullptr;
    popupWindow_ = nullptr;
    aboutWindow_ = nullptr;
}

bool MainWindow::getShowPopupWindow() const {
    return showPopupWindow_;
}

bool MainWindow::getShowAboutWindow() const {
    return showAboutWindow_;
}