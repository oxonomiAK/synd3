#include "tui/panels/popup_window.h"


static int drawErrPopup_ = 0;
void initPopup(WINDOW **popupWindow, WINDOW *main_window)
{
    int maxx, maxy;
    getmaxyx(main_window, maxy, maxx);
    *popupWindow = newwin(POPUP_WIN_HEIGHT, POPUP_WIN_LENGHT, (maxy-POPUP_WIN_HEIGHT)/2, (maxx-POPUP_WIN_LENGHT)/2);
}

void drawPopup(WINDOW *popupWindow, d_arr *processes, ptParams *ptPr)
{
    int maxx, maxy;
    getmaxyx(popupWindow, maxy, maxx);
    //  window | start point y axis | start point x axis | what to print | how much print 
    werase(popupWindow);
    box(popupWindow, 0, 0);

    
    char buff[512];
    char buffToPrint[128];
    int height = 1;
    wattron(popupWindow, A_BOLD);
    const char *headerText = drawErrPopup_ ? "Permission denied: cannot kill process.":"Are you sure you want to kill this process?";
    
    if(maxy >= height+2)
        mvwaddnstr(popupWindow, height++, 2, headerText, maxx-POPUP_TEXT_GAP);
    wattroff(popupWindow, A_BOLD);
    
    height++;

    
    sprintf(buff, "PID: %d",  processes->process[ptPr->selectedProcess].pid);
    if(maxy >= height+2)
        mvwaddnstr(popupWindow, height, 2, buff, maxx-2);
    height++;

    sprintf(buff, "Name: %s", processes->process[ptPr->selectedProcess].name);

    if((int)(strlen(buff) >= POPUP_WIN_LENGHT-1)){
        for(int i = 0, j = 0; i<=(int)(strlen(buff)); i++)
        {
            if(i == POPUP_WIN_LENGHT-4)
            {
                buffToPrint[j] = '\0';

                if(maxy >= height+2)
                    mvwaddnstr(popupWindow, height, 2 , buffToPrint, maxx - POPUP_TEXT_GAP);
                height++;
                buffToPrint[0] = buff[i];
                j = 1;
            }else
            {
                buffToPrint[j] = buff[i];
                j++;
            }
        }
        if(maxy >= height+2)
            mvwaddnstr(popupWindow, height, 2 , buffToPrint, maxx - POPUP_TEXT_GAP);
    }else{
        if(maxy >= height+2)
            mvwaddnstr(popupWindow, 4, 2 , buff, maxx - POPUP_TEXT_GAP);
    }

    
    // sprintf(buff, "Height: %d", height);
    // mvwaddnstr(popupWindow, 4, 20, buff, -1);
    // sprintf(buff, "Maxy: %d", maxy);
    // mvwaddnstr(popupWindow, 5, 20, buff, -1);    
    if(maxy-3 >= height && !drawErrPopup_){
        mvwaddnstr(popupWindow, maxy-2, 4, "[Y] Yes", maxx-POPUP_TEXT_GAP);
        mvwaddnstr(popupWindow, maxy-2, maxx - 10, "[N] No", maxx-POPUP_TEXT_GAP);
    }else if(drawErrPopup_ && maxy-3 >= height){
        mvwaddnstr(popupWindow, maxy-2, 13, "Press any key to close this window!", maxx-POPUP_TEXT_GAP-11);
    }


    wnoutrefresh(popupWindow);
}

void drawErrPopup()
{
    drawErrPopup_ = 1;
}

void removeErrPopup()
{
    drawErrPopup_ = 0;
}