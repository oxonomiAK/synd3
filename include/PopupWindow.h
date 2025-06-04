#ifndef POPUP_WINDOW_H
#define POPUP_WINDOW_H

#include "Window.h"
#include <ncurses.h>
#include <string>
#include <vector>
#include "core/process.h"

class PopupWindow : public Window {
public:
    PopupWindow(WINDOW* parent, int height, int width, int starty, int startx,
                std::string message, std::vector<Process>& processes, bool showConfirmation = true);
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
    void killProcess();

private:
    std::vector<Process> &processes;
    bool showConfirmation_;
    bool visible_;
    std::string message_;
    WINDOW* parent_;       
    WINDOW* win_;
    int selectedProcess_ = 0;
    int currentPID;
    int desiredHeight_;
    int desiredWidth_;
    int startY_;
    int startX_;
    std::string currentName;
};

#endif // POPUP_WINDOW_H
