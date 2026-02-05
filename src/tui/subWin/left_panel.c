#include "tui/panels/left_panel.h"

void initLeftPanel(WINDOW **left_panel, WINDOW *main_window)
{
    int maxx, maxy;
    getmaxyx(main_window, maxy, maxx);
    *left_panel = newwin(maxy - 2, maxx <= LEFT_PANEL_WIDTH ? maxx - 2 : LEFT_PANEL_WIDTH, 1, 1);
}

void drawLeftPanel(WINDOW *left_panel, d_arr *procesess, SysStatistics Sys)
{
    int maxx, maxy;
    getmaxyx(left_panel, maxy, maxx);
    int line = 0;
    char buff[128] = {'\0'};
    
    if(getmaxy(stdscr) >= 5 && getmaxx(left_panel) > 1){
        werase(left_panel);


        // Draw vertical separator line
        for (int y = 0; y < maxy; y++)
        {
            mvwaddch(left_panel, y, LEFT_PANEL_WIDTH-1, ACS_VLINE);
        }
        
        mvwprintw(left_panel, line++, 1, "System Information: ");

        line++;
        
        mvwprintw(left_panel, line, 1, "CPU: ");

        int wordCount = 0;
        char nameByWords[16][16] = {'\0'};
        for (int i = 0, j = 0; i <= (int)(strlen(Sys.cpuName)); i++) {
            // Initialize column index of newString array
            // If space or NULL found, assign NULL into newString[ctr]
            if (Sys.cpuName[i] == ' ' || Sys.cpuName[i] == '\0') {
                nameByWords[wordCount][j] = ' '; // Null-terminate the word
                nameByWords[wordCount][j+1] = '\0'; 
                wordCount++;  // Move to the next word
                j = 0;  // Reset column index to 0 for the next word
            } else {
                nameByWords[wordCount][j] = Sys.cpuName[i]; // Store the character into newString
                j++;  // Move to the next character within the word
            }
        }
        
        for(int i = 0; i <= wordCount; i++)
        {
            int wordLen = strlen(nameByWords[i]);
            if(wordLen + strlen(buff) < LEFT_PANEL_WIDTH-6) // 6 = max 1 pixel gap between line and symbol
            {
                strcat(buff, nameByWords[i]);
            }else{
                mvwaddnstr(left_panel, line, 6, buff, maxx - LEFT_PANEL_WIDTH + strlen(buff)+2);
                buff[0] = '\0';
                strcat(buff, nameByWords[i]);
                line++;
            }
        } 
        mvwaddnstr(left_panel, line++, 6, buff, -1); // print last string because it skips in loop

        line++; // add extra line after CPU name

        mvwprintw(left_panel, line++, 1, "Memory Usage:");
        

        float memUsedInGB = Sys.memUsed / 1024.0f / 1024.0f;
        float memTotalInGB = Sys.memTotal / 1024.0f / 1024.0f;

        float memUsageRatio = memUsedInGB / memTotalInGB;

        int mem_bar_width;
        // Draw memory usage text (MB if <1GB, else GB)
        wattron(left_panel, COLOR_PAIR(MEM_TEXT_COLOR));
        if (memUsedInGB < 1.0f)
            sprintf(buff, " %.0fM of %.1fG", Sys.memUsed / 1024.0f, memTotalInGB);
        else
            sprintf(buff, " %.2fG of %.1fG", memUsedInGB, memTotalInGB);

        mem_bar_width = LEFT_PANEL_WIDTH - strlen(buff) - 5; // 5 = measured to set gap between bar and lable
        mvwaddnstr(left_panel, line, mem_bar_width + 3, buff, maxx - LEFT_PANEL_WIDTH + strlen(buff)+2); // 3 = measured to set gap between lable and line 
         

        int mem_bars = (int)mem_bar_width * memUsageRatio;
        wattron(left_panel, COLOR_PAIR(MEM_BAR_COLOR));

        mvwaddch(left_panel, line, 1, '[');
        int mIter;
        for (mIter = 0; mIter < mem_bar_width; mIter++)
        {
            mvwaddch(left_panel, line, mIter+2,  mIter < mem_bars ? ACS_CKBOARD : ' ');
        }
        mvwaddch(left_panel, line, mIter+2, ']');
        
        

        line += 2;

        wattroff(left_panel, COLOR_PAIR(MEM_TEXT_COLOR));


        // Per-core CPU usage


        wattron(left_panel, COLOR_PAIR(TEXT_COLOR));
        mvwprintw(left_panel, line++, 1, "Per core usage: ");


        for (size_t i = 0; i < Sys.coreCount; i++)
        {

            int wCpu = LEFT_PANEL_WIDTH - 18;
            wattron(left_panel, COLOR_PAIR(CPU_TEXT_COLOR));
            mvwprintw(left_panel, line, 1, "CPU%-2ld:", i);
            wattron(left_panel, COLOR_PAIR(CPU_BAR_COLOR));

            //from 7 to j+11 in mvwaddch: moving cursor to the right
            mvwaddch(left_panel, line, 7, ' '); 
            mvwaddch(left_panel, line, 8, '[');
            int bars = (int)(Sys.CPUpercore[i] * wCpu / 100.0f);
            int j;
            for (j = 0; j < wCpu; j++)
            {
                mvwaddch(left_panel, line, j+9, j < bars ? ACS_CKBOARD : ' ');
            }
            mvwaddch(left_panel, line, j+9, ']');
            mvwaddch(left_panel, line, j+10, ' '); 

            wattron(left_panel, COLOR_PAIR(CPU_TEXT_COLOR));
            sprintf(buff, "%2.1f%%", Sys.CPUpercore[i]);
            mvwaddnstr(left_panel, line, j+11, buff, maxx - LEFT_PANEL_WIDTH + 7); 
            // 7 - another manually measured gap to avoid printing in new line on collapse

            line++;
        }
        wattron(left_panel, COLOR_PAIR(TEXT_COLOR));


        // System info at the bottom

        int total_seconds = (int)(Sys.uptimeSeconds);
        int days = total_seconds / 86400;
        int hours = (total_seconds % 86400) / 3600;
        int minutes = (total_seconds % 3600) / 60;
        int seconds = total_seconds % 60;
        wattron(left_panel, COLOR_PAIR(TEXT_COLOR));


        int bottomCordY[2];
        if(maxy > line + 4)
        {
            int j = 2; // need to reverse top and bottom positions
            for(int i = 0; i<=2; i++){
                bottomCordY[i] = maxy - j-2;
                j--;
            }
                //attached to bottom
        }else{
            for(int i = 0; i<=2; i++)
                bottomCordY[i] = line + i+1;
                //static
        }

        mvwprintw(left_panel, bottomCordY[0], 1, "Uptime: %d days %02d:%02d:%02d", days, hours, minutes, seconds);
        mvwprintw(left_panel, bottomCordY[1], 1, "Tasks: %d, %ld running", procesess->size, Sys.runningTasks);
        mvwprintw(left_panel, bottomCordY[2], 1, "Load avg: %.2f %.2f %.2f", Sys.loadAvg[0], Sys.loadAvg[1], Sys.loadAvg[2]);
        
    }

    wnoutrefresh(left_panel);
}