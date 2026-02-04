#ifndef PROCESS_TABLE_H
#define PROCESS_TABLE_H

#include "core/process.h"
#include "tui/panels/left_panel.h"

#include "tui/colors.h"


#define PID_WIDTH           8
#define PROCESS_LX_OFFS     (PID_WIDTH+1)
#define PROCESS_WIDTH       (44 + PID_WIDTH)
#define FIXED_CPU_X         (PID_WIDTH + 23)      
#define FIXED_MEM_X         (PID_WIDTH + 25)      
#define COLLAPSE_NAME_WIDTH 21      
#define TOTAL_COLUMNS       4


typedef enum{
    PID,
    NAME,
    CPU,
    MEM
}Headers;

typedef struct {
    int colX[TOTAL_COLUMNS];
    int colToPrint[TOTAL_COLUMNS];

    int pidWidth;

    int cpuWidth;
    int cpuValX;

    int memWidth;
    int memValX;

} Layout;

typedef struct{
    int selectedProcess;
    int selectedColumn;
    int scrollOffset;
}ptParams;

void initProcessTable(WINDOW **process_table, WINDOW *main_window);
void drawProcessTable(WINDOW *process_table, d_arr *procesess, SysStatistics Sys, ptParams *ptPr);
void calculateLayout(Layout *l, int maxx, int collapse, SysStatistics Sys);
void calcScrollOffset(int selectedProcess, int* scrollOffset, int maxy, int procSize);




#endif