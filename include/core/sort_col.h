#ifndef SORT_COL_H
#define SORT_COL_H

#include "tui/panels/process_table.h"
#include "core/d_arr.h"

int compPid(const void* a, const void* b);
int compMemUsage(const void* a, const void* b);
int compCpuUsage(const void* a, const void* b);
int compName(const void* a, const void* b);
void sortColumns(ptParams *ptPr, d_arr *procesess);



#endif