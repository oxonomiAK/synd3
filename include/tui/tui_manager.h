#ifndef TUI_MANAGER_H
#define TUI_MANAGER_H

#include "tui/panels/about_window.h"
#include "tui/panels/left_panel.h"
#include "tui/panels/popup_window.h"
#include "tui/panels/process_table.h"
#include "tui/main_window.h"



typedef struct {
    WINDOW *main_win;
    WINDOW *process_table;
    WINDOW *left_panel;
    WINDOW *about;
    WINDOW *popup;
} TUIManager;

void colorPairInit();
void uiInit(TUIManager *wins);
void uiDraw(TUIManager *wins, d_arr *procesess, SysStatistics Sys, ptParams *ptPr);
void resize(TUIManager *wins);
void resizePopup(TUIManager *wins, int maxy, int maxx);
void showAbout();
void hideAbout();
void hidePopup();
void showPopup(TUIManager *wins, int maxy, int maxx);

#endif
