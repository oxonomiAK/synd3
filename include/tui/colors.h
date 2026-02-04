#ifndef COLORS_H
#define COLORS_H

#include <ncurses.h>
#include <unistd.h>
#include <string.h>


typedef enum{
        TITLE_COLOR,
        HEADER_COLOR,
        SELECTED_COLOR,
        PROCESS_COLOR,
        CPU_BAR_COLOR,
        MEM_BAR_COLOR,
        TEXT_COLOR,
        HIGHLIGHT_COLOR,
        CPU_TEXT_COLOR,
        MEM_TEXT_COLOR,
        FOOTER_COLOR,
        YES_COLOR,
        NO_COLOR
}Colors;

#endif