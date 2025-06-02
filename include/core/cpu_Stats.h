#ifndef CPU_STATS_H
#define CPU_STATS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define CPU_STAT_BUFFER_SIZE 1024
#include <stddef.h>    // size_t
    typedef struct
    {
        char cpuName[128];
        float totalUsage;
        float perCore[32];
        size_t coreCount;
        size_t threadCount;
    } CpuStats;

    int get_cpu_stats(CpuStats *out); // neutral func
    void getProcStat(char *buffer, const int size);
    void calcTotalCpuUsage(char *prevBuffer, char *currentBuffer);
    void calcEveryCoreUsage(char *prevBuffer, char *currentBuffer);
    void getTotalCpuUsage();
    void getCpuStats(float *total_usage, float *per_core, size_t size1, size_t size2);
    void CoreCount();
    void getCoreCount(size_t *coreCount, size_t coreCountSize);
    void CpuName();
    void getCpuName(char *cpuName, size_t cpuNameSize);
    void ThreadCount();
    void getThreadCount(size_t *threadCount, size_t threadCountSize);
    unsigned long long calcTotalCpuTck(char *prevBuffer, char *currentBuffer);

#ifdef __cplusplus
}
#endif

#endif