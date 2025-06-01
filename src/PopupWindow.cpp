#include "PopupWindow.h"
#include <ncurses.h>

PopupWindow::PopupWindow(WINDOW* parent, int height, int width, int starty, int startx, std::string message)
    : visible_(false), message_(std::move(message)) {
    int max_y, max_x;
    getmaxyx(parent, max_y, max_x);

    if (starty < 0) starty = (max_y - height) / 2;
    if (startx < 0) startx = (max_x - width) / 2;

    win_ = derwin(parent, height, width, starty, startx);
}


PopupWindow::~PopupWindow() {}

void drawWrappedText(WINDOW* win, int starty, int startx, const std::string& text, int maxWidth) {
    size_t pos = 0;
    int line = 0;
    while (pos < text.length()) {
        std::string lineText = text.substr(pos, maxWidth);
        mvwprintw(win, starty + line, startx, "%s", lineText.c_str());
        pos += maxWidth;
        line++;
    }
}

void PopupWindow::render(const std::vector<Process>& processes) {
    if (!visible_) return;
    
    werase(win_);
    box(win_, 0, 0);

    int max_y, max_x;
    getmaxyx(win_, max_y, max_x);

    wattron(win_, A_BOLD);
    mvwprintw(win_, 1, 2, "%s", message_.c_str());
    wattroff(win_, A_BOLD);

    mvwprintw(win_, 3, 2, "PID: %d", processes[selectedProcess_].getPid());

    std::string nameLine = "Name: " + processes[selectedProcess_].getName();
    drawWrappedText(win_, 4, 2, nameLine, max_x - 4);  

    mvwprintw(win_, max_y - 2, 4, "[Y] Yes");
    mvwprintw(win_, max_y - 2, max_x - 10, "[N] No");

    wrefresh(win_);
}


bool PopupWindow::handleInput(size_t totalProcesses) {
    totalProcesses = totalProcesses;  // Avoid unused parameter warning

    bool confirmed = false;
    while(!confirmed){
        int ch = wgetch(win_); 
        switch (ch) 
        {
            case 'y':
            case 'Y':
                confirmed = true;  
                //killing the process logic should be implemented here
                hide(); 
                return false;  
                
            case 'n':
            case 'N':
                confirmed = true;  
                hide();  
                return false;  
            default:
                continue;
        }
    }
    if (!visible_) return false;
    return false;
    
}

void PopupWindow::show(const std::string& message) {
    message_ = message;
    visible_ = true;
}

void PopupWindow::hide() {
    visible_ = false;
}
void PopupWindow::shutdown() {
    if (win_) {
        delwin(win_);
        win_ = nullptr;
    }
}
void PopupWindow::init() {
    // Initialize the popup window if needed
    // visible_ = false;
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