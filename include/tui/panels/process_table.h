#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

#include "core/process.h"
#include "tui/panels/left_panel.h"

#include "tui/colors.h"


#define GAP                 2
#define PID_WIDTH           8
#define MIN_NAME_WIDTH      21      
#define STR_BUFF_SIZE       512

typedef enum{
    LEFT,
    RIGHT
}Side;

typedef enum{
    //start of left side
    PID,
    //end of left side
    NAME,
    //start of right side
    CPU,
    MEM, 
    //end ofright side
    TOTAL_COLUMNS
}Headers;

typedef enum{
    FIXED,
    AUTO,
    PADDING
}ColumnType;

typedef struct{
    int isMidAligned;
    int isSelectionAffected;

    attr_t attributes;
}ColRenderParams;

typedef struct {
    // header name
    char header[STR_BUFF_SIZE];
    
    //0-left 1-right
    int side;
    /*  
        FIXED - Static header
        AUTO - Dynamic header
        PADDING - To be filled with spaces
    */ 
    int headerType; 
    // desired column width before layout adjustments
    int prefWidth;
    
    int startX;
    // how much symbols print(usually uses in mvwaddnstr)
    int toPrint;

    ColRenderParams renderParams;
}Columns;

typedef struct{
    int selectedProcess;
    int selectedColumn;
    int scrollOffset;
}ptParams;

void configureColumns(Columns *c, SysStat Sys);
void addColumn(Columns *col, int seq, char *h, int side, int prefW, int type, ColRenderParams *renderPrms);
void initProcessTable(WINDOW **process_table, WINDOW *main_window, SysStat Sys);
void drawProcessTable(WINDOW *process_table, d_arr *procesess, SysStat Sys, ptParams *ptPr);
void calculateLayout(Columns *Columns, int maxx);
void calcScrollOffset(int selectedProcess, int* scrollOffset, int maxy, int procSize);
void updateColHeader(Columns *columns, SysStat Sys);
void fillHeaderPadding(Columns *columns);
void fillWithSpaces(Columns *columns, char *str, int i);
void setColumnDataPos(Columns *columns, int i, int *startX, int *toPrint, char *str);


#endif