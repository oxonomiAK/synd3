#include "tui/panels/about_window.h"

static int heightOffset = 0;
static int canScrollU = 0;
static int canScrollD = 0;

void initAbout(WINDOW **about, WINDOW *main_window)
{
    int maxx, maxy;
    getmaxyx(main_window, maxy, maxx);
    *about = newwin(maxy, maxx , 0, 0);
}


void drawAbout(WINDOW *about)
{
    const char *aboutText = 
"(--------------------[ HELP - SYND3 ]--------------------)\n"
"SYND3 is a terminal-based system monitor for Linux.\n"
"\n"
"KEYBOARD SHORTCUTS:\n"
"F1 - Open this help screen\n"
"F9 - Kill selected process (confirmation required)\n"
"F10 - Exit program safely\n"
"\n"
"PROCESS TABLE:\n"
"- Shows list of active processes\n"
"- Columns: PID, NAME, CPU%, MEM%\n"
"- Use UP/DOWN arrows to navigate\n"
"- Sorting by PID, NAME or CPU% (future feature)\n"
"\n"
"SYSTEM PANEL (LEFT SIDE):\n"
"- Memory usage: Total and percentage\n"
"- Per-core CPU usage bar\n"
"- Uptime, Load Average, and running tasks\n"
"\n"
"HOW TO KILL A PROCESS:\n"
"1. Use arrow keys to select the process\n"
"2. Press F9\n"
"3. Confirm with 'Y' or cancel with 'N'\n"
"\n"
"EXITING THE PROGRAM:\n"
"- Press F10 or Ctrl+C to exit\n"
"- Resources are released safely\n"
"- Terminal is restored to its original state\n"
"\n"
"NOTES:\n"
"- UI adapts to terminal size dynamically\n"
"- All colors are currently static (no color load levels)\n"
"- Works in most modern terminals including WSL2\n"
"\n"
"(--------------------Press any key to return --------------------)\n";
//------------------------------------------------------------------------------

    werase(about);
    int maxy, maxx;
    char buff[256];
    getmaxyx(about, maxy, maxx);
    
    

    box(about, 0, 0);
    const char *title = "About";
    if(getmaxy(stdscr) != 2 && maxx > 1)
        mvwaddnstr(about, 1, (maxx - (int)strlen(title)) / 2, title, -1);
    

    int height = 3;
    int startPoint = 0;
    for(int i = 0, j = 0; i<=(int)(strlen(aboutText)); i++)
    {
        if(aboutText[i] == '\n')
        {
            buff[j] = '\0';

            
            //static placement : dynamic placement
            startPoint = maxx - (int)strlen(buff) <= 1 ? 1 : (maxx - (int)strlen(buff)) / 2;

            if(maxy >= height+4-heightOffset && height >= heightOffset + 3)
                mvwaddnstr(about, height-heightOffset, startPoint , buff, maxx - 2);

            buff[0] = '\0';
            height++;
            j = 0;
        }else
        {
            buff[j] = aboutText[i];
            j++;
        }
    }


    canScrollU = heightOffset < height - maxy + 3 ? 1:0;

    canScrollD = heightOffset >= 1 ? 1:0;


    if(maxy > height + 3)
        resetAboutScrollOffs();

    //footer

    const char *footer = "ArrowUp/ArrowDown or Mouse Scroll to scroll, any key to close";
    if(getmaxy(stdscr) >= 4 && maxx > 2){
        startPoint = maxx - (int)strlen(footer) <= 1 ? 1 : (maxx - (int)strlen(footer)) / 2;
        mvwaddnstr(about, maxy - 2, startPoint, footer, maxx - 2);
    }





    wnoutrefresh(about);
}

void scrollAboutU()
{
    if(canScrollU)
        heightOffset++;
}
void scrollAboutD()
{
    if(canScrollD)
        heightOffset--;
}

void resetAboutScrollOffs()
{
    heightOffset = 0;
}