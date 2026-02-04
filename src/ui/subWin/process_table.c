#include "tui/panels/process_table.h"


void initProcessTable(WINDOW **process_table, WINDOW *main_window)
{
    int maxx, maxy;
    getmaxyx(main_window, maxy, maxx);
    *process_table = newwin(maxy-2, maxx >= LEFT_PANEL_WIDTH ? maxx-LEFT_PANEL_WIDTH-2: LEFT_PANEL_WIDTH , 1, LEFT_PANEL_WIDTH + 1);
}

void drawProcessTable(WINDOW *process_table, d_arr *procesess, SysStatistics Sys, ptParams *ptPr)
{
    Layout layout;
    int maxx, maxy;
    int collapse;
    char cpuHeader[16], memHeader[16], pid[PID_WIDTH], name[512], buff[32];
    char *headers[TOTAL_COLUMNS] = {pid, name, cpuHeader, memHeader};
    int y = 1;

    if(getmaxy(stdscr) >= 5 && getmaxx(process_table) > 1){
        werase(process_table);
        getmaxyx(process_table, maxy, maxx);
        collapse = (maxx > PROCESS_WIDTH) ? 0 : 1;

        calculateLayout(&layout, maxx, collapse, Sys);
        
        //  window | start point y axis | start point x axis | what to print | how much print 
        sprintf(headers[PID], "%-*s", layout.pidWidth-1, "PID" );
        sprintf(headers[NAME], "%-*s", layout.colToPrint[NAME], "NAME");
        
        sprintf(headers[MEM], "MEM%% %.2f", Sys.percentMemUsed);
        sprintf(headers[CPU], "CPU%% %.2f", Sys.CPUtotal);

        //=======================================
        //print all headers
        
        for(int i = 0; i<=TOTAL_COLUMNS; i++){
            if(i == ptPr->selectedColumn)
                wattron(process_table, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
            else
                wattron(process_table, COLOR_PAIR(HEADER_COLOR));

            mvwaddnstr(process_table, 0, layout.colX[i],  headers[i], layout.colToPrint[i]);
            wattroff(process_table, A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR));
        }
        wattroff(process_table, COLOR_PAIR(HEADER_COLOR));
        //======================================
        
        for(int i = ptPr->scrollOffset; i<procesess->size; i++, y++)
        {

            //print under MEM%
            sprintf(buff, "%.2f", procesess->process[i].mem_usage);
            wattron(process_table, COLOR_PAIR(MEM_TEXT_COLOR));
            mvwaddnstr(process_table, y, layout.memValX, buff, -1);
            //print uder CPU%
            wattron(process_table, COLOR_PAIR(CPU_TEXT_COLOR));
            sprintf(buff, "%.2f", procesess->process[i].cpu_usage);
            mvwaddnstr(process_table, y, layout.cpuValX, buff, -1);
                
            wattroff(process_table, COLOR_PAIR(CPU_TEXT_COLOR));

            //print under NAME
            if(i == ptPr->selectedProcess)
                wattron(process_table, COLOR_PAIR(SELECTED_COLOR));

            sprintf(name, "%-*s", layout.colToPrint[NAME], procesess->process[i].name);
            mvwaddnstr(process_table, y, PROCESS_LX_OFFS, name, layout.colToPrint[NAME]);
            //print under PID

            sprintf(pid, "%-*d", layout.pidWidth+1, procesess->process[i].pid);
            mvwaddnstr(process_table, y, 0, pid, maxx);
            wattroff(process_table, COLOR_PAIR(SELECTED_COLOR));
            
        }
        
        //if last selected process dissapears selected process will be last existing process
        if(ptPr->selectedProcess >= procesess->size)
            ptPr->selectedProcess = procesess->size-1;

            
        //debug info

        // sprintf(buff, "ScrollOffset: %d", ptPr->scrollOffset);
        // mvwaddnstr(process_table, 10, 50, buff, -1);
        // sprintf(buff, "Vis.Lines: %d", maxy-1);
        // mvwaddnstr(process_table, 11, 50, buff, -1);
        // sprintf(buff, "Proc. size: %d", procesess->size);
        // mvwaddnstr(process_table, 12, 50, buff, -1);
        // sprintf(buff, "Select. proc.: %d", ptPr->selectedProcess);
        // mvwaddnstr(process_table, 13, 50, buff, -1);

        wnoutrefresh(process_table);
        calcScrollOffset(ptPr->selectedProcess, &ptPr->scrollOffset, maxy, procesess->size);
    }
    
}

void calculateLayout(Layout *l, int maxx, int collapse, SysStatistics Sys)
{
    l->colX[PID] = 0;
    l->colToPrint[PID] = maxx;
    l->pidWidth = PID_WIDTH;

    // dynamic widths
    char memHeader[16], cpuHeader[16];
    sprintf(memHeader, "MEM%% %.2f", Sys.percentMemUsed);
    sprintf(cpuHeader, "CPU%% %.2f", Sys.CPUtotal);

    l->memWidth = strlen(memHeader);
    l->cpuWidth = strlen(cpuHeader);

    if (collapse) {
        l->colX[MEM] = FIXED_MEM_X + l->cpuWidth;
        l->colToPrint[MEM] = maxx - l->colX[MEM];

        l->colX[CPU] = FIXED_CPU_X;
        l->colToPrint[CPU] = maxx - l->colX[CPU];
        
        l->colX[NAME] = l->pidWidth + 1;
        l->colToPrint[NAME] = (maxx < l->pidWidth + COLLAPSE_NAME_WIDTH) ? maxx - PID_WIDTH : COLLAPSE_NAME_WIDTH;
    } else {
        l->colX[MEM] = maxx - l->memWidth;
        l->colToPrint[MEM] = -1;

        l->colX[CPU] = l->colX[MEM] - l->cpuWidth - 2;
        l->colToPrint[CPU] = -1;

        l->colX[NAME] = l->pidWidth + 1;
        l->colToPrint[NAME] = l->colX[CPU] - l->colX[NAME] - 2;
        
    }
    
    l->memValX = l->colX[MEM] + 3;
    l->cpuValX = l->colX[CPU] + 3;
}

void calcScrollOffset(int selectedProcess, int* scrollOffset, int maxy, int procSize){
    int visibleLines = maxy-1;
    
    //scroll screen down
    if (selectedProcess >= *scrollOffset + visibleLines && selectedProcess < procSize)
    {
        *scrollOffset = selectedProcess-maxy+2;
    }
    //scroll screen up
    if(selectedProcess < *scrollOffset)
    {
        *scrollOffset = selectedProcess;
    }

    //attach last process in list to bottom 
    if(visibleLines <= procSize){
        while(*scrollOffset + visibleLines > procSize) {
            *scrollOffset -=1;
        }
    }

}

