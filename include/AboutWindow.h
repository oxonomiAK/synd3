#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

#include "Window.h"
#include <ncurses.h>
#include <string>

class AboutWindow : public Window {
public:
    AboutWindow(WINDOW* parent);
    ~AboutWindow();

    void init() override;
    void render(const std::vector<Process>& processes) override;
    bool handleInput(size_t totalProcesses) override;
    void shutdown() override;
    void show();
    void hide();
    bool isVisible() const;

private:
    WINDOW* win_ = nullptr;
    bool visible_ = false;
    std::string aboutText_;
    int scrollOffset_ = 0;
};

#endif // ABOUT_WINDOW_H
