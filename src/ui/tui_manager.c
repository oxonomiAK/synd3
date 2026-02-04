#include "tui/tui_manager.h"


static int showAbout_ = 0;
static int showPopup_ = 0;
void uiInit(TUIManager *wins)
{

    colorPairInit();
    initMainWindow(&wins->main_win);
    initProcessTable(&wins->process_table, wins->main_win);
    initLeftPanel(&wins->left_panel, wins->main_win);
    initAbout(&wins->about, wins->main_win);
    initPopup(&wins->popup, wins->main_win);

}


void uiDraw(TUIManager *wins, d_arr *procesess, SysStatistics Sys, ptParams *ptPr)
{
    if(showAbout_)
    {
        drawAbout(wins->about);
        doupdate();
        return;
    }

    drawMainWindow(wins->main_win);
    drawProcessTable(wins->process_table, procesess, Sys, ptPr);
    drawLeftPanel(wins->left_panel, procesess, Sys);
    if(showPopup_){
        if(getmaxy(stdscr) > 3 && getmaxx(wins->popup) >= 2)
            drawPopup(wins->popup, procesess, ptPr);

    }
    doupdate();
}

void resize(TUIManager *wins)
{
    int maxy, maxx;
    getmaxyx(wins->main_win, maxy, maxx);
    static int collapsedTerminal;
    
    if(maxy <= 3)
    {
        mvwin(wins->left_panel, 0, 0);
        mvwin(wins->process_table, 0, 0);
        werase(wins->left_panel);
        werase(wins->process_table);
        collapsedTerminal = 1;
        return;
    }else if(collapsedTerminal && maxy > 4)
    {
        mvwin(wins->left_panel, 1, 1);
        mvwin(wins->process_table, 1, LEFT_PANEL_WIDTH + 1);
        collapsedTerminal = 0;
    }

    wresize(wins->process_table, maxy - 2, maxx-LEFT_PANEL_WIDTH-2);
    wresize(wins->left_panel, maxy - 2, maxx <= LEFT_PANEL_WIDTH ? maxx - 2 : LEFT_PANEL_WIDTH);
    
    if(showAbout_)
    {
        wresize(wins->about, maxy, maxx);
        wnoutrefresh(wins->about);
    }
    
    if(showPopup_)
        resizePopup(wins, maxy, maxx);


    
    wnoutrefresh(wins->main_win);
    wnoutrefresh(wins->process_table);
    wnoutrefresh(stdscr);
    doupdate();
}

void resizePopup(TUIManager *wins, int maxy, int maxx)
{
    //a lot of manually measured values to correctly move this popup
    if(POPUP_WIN_LENGHT >= maxx-3)
        wresize(wins->popup, POPUP_WIN_HEIGHT, maxx-4);
    else   
        wresize(wins->popup, POPUP_WIN_HEIGHT, POPUP_WIN_LENGHT);

    if(POPUP_WIN_HEIGHT >= maxy-1)
        wresize(wins->popup, maxy-2, POPUP_WIN_LENGHT);

    if(POPUP_WIN_HEIGHT >= maxy-1 && POPUP_WIN_LENGHT >= maxx-3){
        wresize(wins->popup, maxy-2, maxx-4);
    }

    if(maxx-POPUP_WIN_LENGHT <= 4)
        mvwin(wins->popup, (maxy-POPUP_WIN_HEIGHT)/2, 2);
    else if(maxy - POPUP_WIN_HEIGHT <= 2)
        mvwin(wins->popup, 1, (maxx-POPUP_WIN_LENGHT)/2);
    else
        mvwin(wins->popup, (maxy-POPUP_WIN_HEIGHT)/2, (maxx-POPUP_WIN_LENGHT)/2);

    if(maxy - POPUP_WIN_HEIGHT <= 2 && maxx-POPUP_WIN_LENGHT <= 4)
        mvwin(wins->popup, 1, 2);

}

void colorPairInit()
{
    init_pair(TITLE_COLOR, 10, COLOR_CYAN);
    init_pair(HEADER_COLOR, 144, 17);
    init_pair(SELECTED_COLOR, COLOR_WHITE, COLOR_BLUE);
    init_pair(PROCESS_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(CPU_BAR_COLOR, COLOR_BLACK, COLOR_GREEN);
    init_pair(MEM_BAR_COLOR, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(TEXT_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(HIGHLIGHT_COLOR, 15, COLOR_BLUE);
    init_pair(CPU_TEXT_COLOR, COLOR_CYAN, COLOR_BLACK);
    init_pair(MEM_TEXT_COLOR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(FOOTER_COLOR, COLOR_CYAN, COLOR_BLACK);
}

void showAbout()
{
    showAbout_ = 1;
}

void hideAbout()
{
    showAbout_ = 0;
}

void showPopup(TUIManager *wins, int maxy, int maxx)
{
    resizePopup(wins, maxy, maxx);
    showPopup_ = 1;
}

void hidePopup()
{
    showPopup_ = 0;
}
