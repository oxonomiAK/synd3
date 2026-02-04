#ifndef ABOUT_H
#define ABOUT_H

#include <ncurses.h>
#include <unistd.h>
#include <string.h>


void initAbout(WINDOW **popup, WINDOW *main_window);
void drawAbout(WINDOW *popup);
void scrollAboutD();
void scrollAboutU();
void resetAboutScrollOffs();

#endif