#ifndef POPUP_WINDOW_H
#define POPUP_WINDOW_H

#define POPUP_WIN_HEIGHT 10
#define POPUP_WIN_LENGHT 60
#define POPUP_TEXT_GAP 4
#include <ncurses.h>
#include "core/d_arr.h"
#include "tui/panels/process_table.h"


void initPopup(WINDOW **popupWindow, WINDOW *main_window);
void drawPopup(WINDOW *popupWindow, d_arr *processes, ptParams *ptPr);
void removeErrPopup();
void drawErrPopup();

#endif