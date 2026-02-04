#include "core/input_handler.h"
#include "app.h"


void handle_winch(TUIManager wins) 
{
    resize(&wins);
}


int handleMainInput(int *selectedColumn, int *selectedPorcess, d_arr processes, TUIManager wins)
{
    
    static int aboutActive = 0;
    static int popupActive = 0;

    if(aboutActive)
    {
        aboutActive = handleAboutInput(wins);
        return 1;
    }
    if(popupActive)
    {
        popupActive = handlePopupInput(wins, *selectedPorcess, processes);
        return 1;
    }

    MEVENT event;
    int ch = wgetch(stdscr);
    switch(ch)
    {
        case KEY_RESIZE:
            handle_winch(wins);
            break;
        case KEY_UP:
            if(*selectedPorcess > 0)
                *selectedPorcess-=1;
            break;
        case KEY_DOWN:
            if(*selectedPorcess < processes.size -1)
                *selectedPorcess+=1;
            break;
        case KEY_LEFT:
            *selectedColumn= (*selectedColumn - 1 + TOTAL_COLUMNS) % TOTAL_COLUMNS;
            break;
        case KEY_RIGHT:
            *selectedColumn = (*selectedColumn+ 1) % TOTAL_COLUMNS;
            break;
        case KEY_F(9):
            showPopup(&wins, getmaxy(wins.main_win), getmaxx(wins.main_win));
            noUpdate();
            popupActive = 1;
            break;
        case KEY_F(10):
            return 0;
            
        case KEY_F(1):
            showAbout();
            resetAboutScrollOffs();
            aboutActive = 1;
            break;
        case KEY_MOUSE:
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON4_PRESSED)
                {
                    if (*selectedPorcess > 0)
                        *selectedPorcess-=1;
                }
                else if (event.bstate & BUTTON5_PRESSED)
                {
                    if (*selectedPorcess < processes.size - 1)
                        *selectedPorcess+=1;
                }
            }
            break;
        case -1:
            break;
        default:
            break;
    }
    return 1;
       
}


int handleAboutInput(TUIManager wins){
    int ch = wgetch(stdscr);
    MEVENT event;
    switch(ch)
    {
        case KEY_RESIZE:
            handle_winch(wins);
            break;
        case KEY_UP:
            scrollAboutU();
            break;
        case KEY_DOWN:
            scrollAboutD();
            break;
            
        case KEY_MOUSE:
            if (getmouse(&event) == OK)
            {
                if (event.bstate & BUTTON4_PRESSED)
                {
                    scrollAboutD();
                }
                else if (event.bstate & BUTTON5_PRESSED)
                {
                    scrollAboutU();
                }
            }
            break;
        case -1:
            break;
        default:
            hideAbout();
            return 0;
            break;
    }
    return 1;

}

int handlePopupInput(TUIManager wins, int selectedProcess, d_arr processes){
    int ch = wgetch(stdscr);
    static int errPopupActivated = 0;
    switch(ch)
    {
        case KEY_RESIZE:
            handle_winch(wins);
            break;
        case 'y':
        case 'Y':
            if(!killProcess(processes, selectedProcess) && !errPopupActivated)
            {
                drawErrPopup();
                errPopupActivated = 1;
                return 1;
            }

            removeErrPopup();
            hidePopup();
            resumeUpdate();
            return 0;
            break;
        case 'N':
        case 'n':
            hidePopup();
            resumeUpdate();
            return 0;
            break;
        case -1:
        case KEY_MOUSE:
            break;
        default:
            if(errPopupActivated)
            {
                removeErrPopup();
                hidePopup();
                resumeUpdate();
                errPopupActivated = 0;
                return 0;
            }
            break;
    }
    return 1;
}