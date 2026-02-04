#ifndef LEFT_PANEL_H
#define LEFT_PANEL_H

#define LEFT_PANEL_WIDTH 35


#include "tui/colors.h"
#include "core/process.h"

void initLeftPanel(WINDOW **left_panel, WINDOW *main_window);
void drawLeftPanel(WINDOW *process_table, d_arr *procesess, SysStatistics Sys);



#endif 