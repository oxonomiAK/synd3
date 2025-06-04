#include "PopupWindow.h"
#include <ncurses.h>
#include <string>
#include <vector>
#include <csignal>
#include <cerrno>     // errno
#include <cstring>    // strerror
#include <iostream>
// Helper function to draw wrapped text inside the window
static void drawWrappedText(WINDOW* win, int starty, int startx, const std::string& text, int maxWidth) {
    size_t pos = 0;
    int line = 0;
    while (pos < text.length()) {
        std::string lineText = text.substr(pos, maxWidth);
        mvwprintw(win, starty + line, startx, "%s", lineText.c_str());
        pos += maxWidth;
        line++;
    }
}

PopupWindow::PopupWindow(WINDOW* parent, int height, int width, int starty, int startx,
                         std::string message, std::vector<Process>& processes, bool showConfirmation)
    : processes(processes), showConfirmation_(showConfirmation), visible_(false), message_(std::move(message)), parent_(parent), win_(nullptr),
      desiredHeight_(height), desiredWidth_(width),
      startY_(starty), startX_(startx)
{
    // If negative start positions provided, center the window on the parent
    if (startY_ < 0 || startX_ < 0) {
        int max_y, max_x;
        getmaxyx(parent_, max_y, max_x);
        if (startY_ < 0) startY_ = (max_y - desiredHeight_) / 2;
        if (startX_ < 0) startX_ = (max_x - desiredWidth_) / 2;
    }
    // Create the derived window initially
    win_ = derwin(parent_, desiredHeight_, desiredWidth_, startY_, startX_);
}

PopupWindow::~PopupWindow() {
    if (win_) {
        delwin(win_);
        win_ = nullptr;
    }
}

void PopupWindow::render(const std::vector<Process>& processes) {
    if (!visible_) return;

    int max_y, max_x;
    getmaxyx(parent_, max_y, max_x);

    // Recalculate window position to keep it centered relative to the parent
    int new_starty = (max_y - desiredHeight_) / 2;
    int new_startx = (max_x - desiredWidth_) / 2;

    // Delete old window before recreating with updated position and size
    if (win_) {
        delwin(win_);
        win_ = nullptr;
    }
    win_ = derwin(parent_, desiredHeight_, desiredWidth_, new_starty, new_startx);

    werase(win_);
    box(win_, 0, 0);

    wattron(win_, A_BOLD);
    mvwprintw(win_, 1, 2, "%s", message_.c_str());
    wattroff(win_, A_BOLD);

    mvwprintw(win_, 3, 2, "PID: %d", processes[selectedProcess_].getPid());

    currentName = processes[selectedProcess_].getName();
    std::string nameLine = "Name: " + processes[selectedProcess_].getName();
    drawWrappedText(win_, 4, 2, nameLine, desiredWidth_ - 4);

    if (showConfirmation_) {
        mvwprintw(win_, desiredHeight_ - 2, 4, "[Y] Yes");
        mvwprintw(win_, desiredHeight_ - 2, desiredWidth_ - 10, "[N] No");
    }

    wrefresh(win_);
}
void PopupWindow::killProcess() {

    if (kill(processes[selectedProcess_].getPid(), SIGTERM) == 0) {

        return;
    } else {
        int err = errno;
        if (err == EPERM) {
            PopupWindow* errorWindow = new PopupWindow(parent_, 7, 50, -1, -1,
                                                       "",
                                                       processes, false); 
            errorWindow->show("Permission denied to kill process: ");
            errorWindow->render(processes);
            wgetch(errorWindow->win_); 
            delete errorWindow;

        return;
        }
    }
}

bool PopupWindow::handleInput(size_t /*totalProcesses*/) {
    bool confirmed = false;
    while (!confirmed) {
        int ch = wgetch(win_);
        switch (ch) {
            case 'y':
            case 'Y':
                confirmed = true;
                killProcess();
                hide();
                return false; // Close popup window
            case 'n':
            case 'N':
                confirmed = true;
                hide();
                return false; // Close popup window
            default:
                continue;
        }
    }
    return false;
}

void PopupWindow::show(const std::string& message) {
    message_ = message;
    visible_ = true;
}

void PopupWindow::hide() {
    visible_ = false;
    if (win_) {
        werase(win_);
        wrefresh(win_);
    }
}

void PopupWindow::shutdown() {
    if (win_) {
        delwin(win_);
        win_ = nullptr;
    }
}

void PopupWindow::init() {
    // Optional initialization code
}

bool PopupWindow::isVisible() const {
    return visible_;
}

void PopupWindow::setSelectedProcess(int idx) {
    selectedProcess_ = idx;
}

int PopupWindow::getSelectedProcess() const {
    return selectedProcess_;
}
