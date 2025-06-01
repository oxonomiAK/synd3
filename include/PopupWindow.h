#ifndef POPUP_WINDOW_H
#define POPUP_WINDOW_H

#include "Window.h"
#include <ncurses.h>

class PopupWindow : public Window {
public:
    PopupWindow(WINDOW* parent, int height, int width, int starty, int startx, std::string message );
    ~PopupWindow();

    void render(const std::vector<Process>& processes) override;
    bool handleInput(size_t totalProcesses) override;

    void show(const std::string& message);
    void hide();
    bool isVisible() const;
    void shutdown() override;
    void init() override;
    void setSelectedProcess(int idx);
    int getSelectedProcess() const;
private:
    bool visible_;
    std::string message_;
    WINDOW* win_;
    int selectedProcess_ = 0; 
};

#endif // POPUP_WINDOW_H
