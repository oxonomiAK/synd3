#include "AboutWindow.h"
#include <ncurses.h>
#include <cstring>

AboutWindow::AboutWindow(WINDOW *parent) : visible_(false), scrollOffset_(0)
{
    int max_y, max_x;
    getmaxyx(parent, max_y, max_x);
    win_ = derwin(parent, max_y, max_x, 0, 0);

    aboutText_ = R"(        (--------------------[ HELP - SYND3 ]--------------------)
        SYND3 is a terminal-based system monitor for Linux.

        KEYBOARD SHORTCUTS:
          F1 - Open this help screen
          F9 - Kill selected process (confirmation required)
          F10 - Exit program safely

        PROCESS TABLE:
          - Shows list of active processes
          - Columns: PID, NAME, CPU%, MEM%
          - Use UP/DOWN arrows to navigate
          - Sorting by PID, NAME or CPU% (future feature)

        SYSTEM PANEL (LEFT SIDE):
          - Memory usage: Total and percentage
          - Per-core CPU usage bar
          - Uptime, Load Average, and running tasks

        HOW TO KILL A PROCESS:
          1. Use arrow keys to select the process
          2. Press F9
          3. Confirm with 'Y' or cancel with 'N'

        EXITING THE PROGRAM:
          - Press F10 or Ctrl+C to exit
          - Resources are released safely
          - Terminal is restored to its original state

        NOTES:
          - UI adapts to terminal size dynamically
          - All colors are currently static (no color load levels)
          - Works in most modern terminals including WSL2

        (--------------------Press any key to return --------------------))";
}

AboutWindow::~AboutWindow()
{
    shutdown();
}

void AboutWindow::init()
{
    shutdown();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    win_ = derwin(stdscr, max_y, max_x, 0, 0);
    scrollOffset_ = 0;

    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
}

void AboutWindow::render(const std::vector<Process> &)
{
    if (!visible_ || !win_)
        return;
    werase(win_);
    box(win_, 0, 0);

    int max_y, max_x;
    getmaxyx(win_, max_y, max_x);

    const char *title = "About";
    mvwprintw(win_, 1, (max_x - (int)strlen(title)) / 2, "%s", title);

    int availableHeight = max_y - 5;
    int shift = 4;

    std::vector<std::string> lines;
    size_t pos = 0, next;
    while ((next = aboutText_.find('\n', pos)) != std::string::npos)
    {
        lines.emplace_back(aboutText_.substr(pos, next - pos));
        pos = next + 1;
    }
    if (pos < aboutText_.size())
        lines.emplace_back(aboutText_.substr(pos));

    if (scrollOffset_ > (int)lines.size() - availableHeight)
        scrollOffset_ = std::max(0, (int)lines.size() - availableHeight);

    int line = 3;
    for (int i = scrollOffset_; i < (int)lines.size() && line < max_y - 2; ++i)
    {
        int padding = ((max_x - (int)lines[i].length()) / 2) - shift;
        if (padding < 0)
            padding = 0;
        mvwprintw(win_, line++, padding, "%s", lines[i].c_str());
    }

    const char *footer = "ArrowUp/ArrowDown or Mouse Scroll to scroll, any key to close";
    mvwprintw(win_, max_y - 2, (max_x - (int)strlen(footer)) / 2, "%s", footer);

    wrefresh(win_);
}

bool AboutWindow::handleInput(size_t)
{
    if (!visible_ || !win_)
        return false;

    keypad(win_, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);
    int ch;

    MEVENT event;
    while ((ch = wgetch(win_)))
    {
        switch (ch)
        {
        case KEY_UP:
            if (scrollOffset_ > 0)
                --scrollOffset_;
            render({});
            break;
        case KEY_DOWN:
            ++scrollOffset_;
            render({});
            break;
        case KEY_MOUSE:
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON4_PRESSED)
                {
                    if (scrollOffset_ > 0)
                        --scrollOffset_;
                    render({});
                }
                else if (event.bstate & BUTTON5_PRESSED)
                {
                    ++scrollOffset_;
                    render({});
                }
            }
            break;
        default:
            hide();
            return false;
        }
    }

    return false;
}

void AboutWindow::show()
{
    visible_ = true;
}

void AboutWindow::hide()
{
    visible_ = false;
    if (win_)
    {
        werase(win_);
        wrefresh(win_);
    }
}

bool AboutWindow::isVisible() const
{
    return visible_;
}

void AboutWindow::shutdown()
{
    if (win_)
    {
        delwin(win_);
        win_ = nullptr;
    }
}
