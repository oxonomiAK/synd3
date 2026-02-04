#ifndef PROCESS_H
#define PROCESS_H

#include "core/process_Info.h"
#include "core/d_arr.h"

typedef struct{
    float CPUtotal, loadAvg[3], CPUpercore[32], memTotal, memUsed, percentMemUsed;
    size_t runningTasks, totalTasks, coreCount;
    double uptimeSeconds;
    char cpuName[64];
}SysStatistics;
d_arr fetchProcesses();

#endif