#include "tui/panels/process_table.h"

static Columns columns[TOTAL_COLUMNS];
void initProcessTable(WINDOW **process_table, WINDOW *main_window, SysStat Sys)
{
    int maxx, maxy;
    getmaxyx(main_window, maxy, maxx);
    *process_table = newwin(maxy-2, maxx >= LEFT_PANEL_WIDTH ? maxx-LEFT_PANEL_WIDTH-GAP: LEFT_PANEL_WIDTH , 1, LEFT_PANEL_WIDTH + 1);
    
    configureColumns(columns, Sys); 
}


void configureColumns(Columns *columns, SysStat Sys){
    char temp[32] = {0};
    ColRenderParams renderParams[TOTAL_COLUMNS] = {0};
    
    //create PID column
    renderParams[PID].isSelectionAffected = 1;
    addColumn(columns, PID, "PID", LEFT, PID_WIDTH, PADDING, &renderParams[PID]);

    //create NAME column
    renderParams[NAME].isSelectionAffected = 1;
    addColumn(columns, NAME, "NAME", LEFT, 0, PADDING, &renderParams[NAME]);

    //create CPU column
    sprintf(temp, "CPU%% %.2f", Sys.CPUtotal);
    renderParams[CPU].isMidAligned = 1;
    renderParams[CPU].attributes = COLOR_PAIR(CPU_TEXT_COLOR);
    addColumn(columns, CPU, temp, RIGHT, 0, AUTO, &renderParams[CPU]);
    
    //create MEM column
    sprintf(temp, "MEM%% %.2f", Sys.percentMemUsed);
    renderParams[MEM].isMidAligned = 1;
    renderParams[MEM].attributes = COLOR_PAIR(MEM_TEXT_COLOR);
    addColumn(columns, MEM, temp, RIGHT, 0, AUTO, &renderParams[MEM]);
}


void drawProcessTable(WINDOW *process_table, d_arr *procesess, SysStat Sys, ptParams *ptPr)
{
    if(getmaxy(stdscr) >= 5 && getmaxx(process_table) > 1){
        int maxx = 0, maxy = 0;
        char buff[STR_BUFF_SIZE] = {0};
        int y = 1;
        attr_t attributes = 0;
        getmaxyx(process_table, maxy, maxx);
        werase(process_table);
        
        
        updateColHeader(columns, Sys);
        calculateLayout(columns, maxx);
        fillHeaderPadding(columns);

    
        //  window | start point y axis | start point x axis | what to print | how much print 
        //=======================================
        //print all headers
        
        for(int i = 0; i<TOTAL_COLUMNS; i++){
            if(i == ptPr->selectedColumn)
                attributes = A_BOLD | COLOR_PAIR(HIGHLIGHT_COLOR);
            else
                attributes = COLOR_PAIR(HEADER_COLOR);

            wattron(process_table, attributes);
            mvwaddnstr(process_table, 0, columns[i].startX, columns[i].header, columns[i].toPrint);
            wattroff(process_table, attributes);
            attributes = 0;
        }
        //======================================
        
        for(int i = ptPr->scrollOffset; i<procesess->size; i++, y++)
        {
            int startx = 0;
            int toPrint = 0;
            for(int j = 0; j<TOTAL_COLUMNS; j++){
                switch (j)
                {
                case MEM:
                    sprintf(buff, "%.2f", procesess->process[i].mem_usage);
                    break;
                
                case CPU:
                    sprintf(buff, "%.2f", procesess->process[i].cpu_usage);
                    break;

                case NAME:         
                    sprintf(buff, "%s", procesess->process[i].name);
                    break;

                case PID:
                    sprintf(buff, "%d", procesess->process[i].pid);
                    break;

                default:
                    continue;
                }

                //filling gaps between columns with spaces to apply colour
                if(columns[j].renderParams.isSelectionAffected)
                    fillWithSpaces(columns, buff, j);

                attributes = columns[j].renderParams.attributes;
                if(i == ptPr->selectedProcess && columns[j].renderParams.isSelectionAffected)
                    attributes =  COLOR_PAIR(SELECTED_COLOR);


                
                setColumnDataPos(columns, j, &startx, &toPrint, buff);
                wattron(process_table, attributes);
                mvwaddnstr(process_table, y, startx, buff, toPrint);
                wattroff(process_table, attributes);
                attributes = 0;
            }
            
        }
        
        //if last selected process dissapears selected process will be last existing process
        if(ptPr->selectedProcess >= procesess->size)
            ptPr->selectedProcess = procesess->size-1;

        wnoutrefresh(process_table);
        calcScrollOffset(ptPr->selectedProcess, &ptPr->scrollOffset, maxy, procesess->size);
    }
    
}



void addColumn(Columns *columns, int seq, char *header, int side, int prefW, int type, ColRenderParams *renderPrms){
    strcpy(columns[seq].header, header);
    columns[seq].side = side;
    columns[seq].headerType = type;
    columns[seq].prefWidth = prefW;
    columns[seq].renderParams = *renderPrms;


}

void calculateLayout(Columns *columns, int maxx)
{
    int currentX = 0;
    int currentRightX = maxx;

    int nameWidth = 0;
    for(int i=0; i<TOTAL_COLUMNS; i++){
        nameWidth+=columns[i].prefWidth+GAP;
    }
    nameWidth-=columns[NAME].prefWidth+GAP;
    nameWidth+=MIN_NAME_WIDTH;

    int collapse = (maxx > nameWidth) ? 0 : 1;

    if (collapse) {
        for(int i=0; i<TOTAL_COLUMNS; i++){
            //special scenarios
            /*
                NAME has its own behavior because it located between left aligned and right aligned columns.
                In the first case it expands if terminal window expands and it shrinks if terminal window shrinks.
                In the second case if NAME width becomes smaller than MIN_NAME_WIDTH it stops shrinking and starts collapsing.
            */
            if(i == NAME){
                columns[NAME].startX = currentX;
                int widhtBeforeName = 0;
                for(int j=0; j<NAME; j++)
                    widhtBeforeName+=columns[j].toPrint+GAP;

                columns[NAME].toPrint = (maxx < widhtBeforeName + MIN_NAME_WIDTH) ? maxx - widhtBeforeName : MIN_NAME_WIDTH;
                currentX += columns[NAME].toPrint + GAP;
                continue;
            }
            if(!columns[i].side){
                columns[i].startX = currentX;
                columns[i].toPrint = columns[i].prefWidth;
                currentX += columns[i].prefWidth + GAP;
            }else{
                columns[i].startX = currentX;
                columns[i].toPrint = maxx - columns[i].startX;
                currentX += columns[i].prefWidth + GAP;
            }

        }
    } else {
        for(int i=0; i<TOTAL_COLUMNS; i++){
            //special scenarios
            if(i == NAME || columns[i].side){
                continue;
            }
            columns[i].startX = currentX;
            columns[i].toPrint = columns[i].prefWidth;
            currentX += columns[i].prefWidth + GAP;
        }
        for(int i=TOTAL_COLUMNS-1; i>=0; i--){

            if(i == NAME || !columns[i].side){
                continue;
            }
            currentRightX -= columns[i].prefWidth;
            columns[i].startX = currentRightX;
            columns[i].toPrint = columns[i].prefWidth;
            currentRightX -= GAP;
        }
        
        //process special scenarios
        //NAME case
        columns[NAME].startX = currentX;
        columns[NAME].toPrint = currentRightX - currentX;
    }
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


void updateColHeader(Columns *columns, SysStat Sys){

    for(int i=0; i<TOTAL_COLUMNS; i++){
        if(columns[i].headerType != AUTO)
            continue;
        if(i == MEM){
            sprintf(columns[i].header, "MEM%% %.2f", Sys.percentMemUsed);
            columns[i].prefWidth = strlen(columns[i].header);
            continue;
        }
        if(i == CPU){
            sprintf(columns[i].header, "CPU%% %.2f", Sys.CPUtotal);
            columns[i].prefWidth = strlen(columns[i].header);
        }
    }
    
}

void fillHeaderPadding(Columns *columns){
    char temp[STR_BUFF_SIZE] = {0};
    for(int i=0; i<TOTAL_COLUMNS; i++){
        if(columns[i].headerType == PADDING){
            sprintf(temp, "%-*s", columns[i].toPrint, columns[i].header);
            strcpy(columns[i].header, temp);
            columns[i].prefWidth = strlen(columns[i].header);
        }
    }
}


void fillWithSpaces(Columns *columns, char *str, int i){
    char buff[STR_BUFF_SIZE] = {0};
    strncpy(buff, str, columns[i].toPrint);

    if(columns[i].renderParams.isMidAligned){
        int len = strlen(buff);
        int freeSpace = columns[i].toPrint - len;

        int toFillLeft = len + freeSpace/2 + freeSpace%2;
        int toFillRight = freeSpace/2;
        if(i < TOTAL_COLUMNS - 1 && columns[i+1].renderParams.isSelectionAffected)
            toFillRight+=GAP;

        sprintf(str, "%*s%-*s", toFillLeft, buff, toFillRight, "");
        return;
    }
    sprintf(str, "%-*s", columns[i].toPrint+GAP, buff);
}

void setColumnDataPos(Columns *columns, int i, int *startX, int *toPrint, char *str){

    if(columns[i].renderParams.isMidAligned && !columns[i].renderParams.isSelectionAffected){
        int len = strlen(str);
        int centerPos = columns[i].startX + (columns[i].prefWidth - len)/2;
        if(!(len%2) && columns[i].prefWidth % 2)
            centerPos += 1;

        *startX = centerPos;
        *toPrint = columns[i].toPrint - (columns[i].prefWidth - len)/2;
        return;
    }
    
    *startX = columns[i].startX;

    //special case
    //filling gaps between columns with spaces to apply colour
    if(i < TOTAL_COLUMNS - 1 && columns[i].renderParams.isSelectionAffected && columns[i+1].renderParams.isSelectionAffected){
        *toPrint = columns[i].toPrint+GAP;
        return;
    }

    *toPrint = columns[i].toPrint;

}
