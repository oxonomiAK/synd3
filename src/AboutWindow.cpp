#include "AboutWindow.h"
#include <ncurses.h>
#include <cstring>

AboutWindow::AboutWindow(WINDOW* parent) : visible_(false) {
    int max_y, max_x;
    getmaxyx(parent, max_y, max_x);
    // Create full-screen derived window covering entire parent
    win_ = derwin(parent, max_y, max_x, 0, 0);

    aboutText_ = "Glance\nVersion 1.0\n(c) 2025 Your Name\n\nA simple process viewer.";
}

AboutWindow::~AboutWindow() {
    shutdown();
}

void AboutWindow::init() {
    shutdown();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    // Create full-screen window covering entire stdscr
    win_ = derwin(stdscr, max_y, max_x, 0, 0);
}

void AboutWindow::render(const std::vector<Process>&) {
    if (!visible_ || !win_) return;
    werase(win_);
    box(win_, 0, 0);
    // Title at top center
    int max_y, max_x;
    getmaxyx(win_, max_y, max_x);
    const char* title = "About";
    mvwprintw(win_, 1, (max_x - (int)strlen(title)) / 2, "%s", title);

    int line = 3;
    size_t pos = 0, next;
    while ((next = aboutText_.find('\n', pos)) != std::string::npos) {
        // Print each line left-aligned with some margin
        mvwprintw(win_, line++, 2, "%s", aboutText_.substr(pos, next - pos).c_str());
        pos = next + 1;
    }
    if (pos < aboutText_.size())
        mvwprintw(win_, line++, 2, "%s", aboutText_.substr(pos).c_str());

    // Instruction at bottom center
    const char* footer = "Press any key to close";
    mvwprintw(win_, max_y - 2, (max_x - (int)strlen(footer)) / 2, "%s", footer);

    wrefresh(win_);
}

bool AboutWindow::handleInput(size_t) {
    if (!visible_ || !win_) return false;
    wgetch(win_); // Wait for any key press to close
    hide();
    return false;
}

void AboutWindow::show() {
    visible_ = true;
}

void AboutWindow::hide() {
    visible_ = false;
        if (win_) {
        werase(win_);
        wrefresh(win_);
    }
}

bool AboutWindow::isVisible() const {
    return visible_;
}

void AboutWindow::shutdown() {
    if (win_) {
        delwin(win_);
        win_ = nullptr;
    }
}
