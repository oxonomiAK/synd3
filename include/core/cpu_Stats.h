#ifndef CPU_STATS_H
#define CPU_STATS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define CPU_STAT_BUFFER_SIZE 1024
#include <stddef.h> // size_t
    typedef struct
    {
        float loadAvg[3];
        size_t runningTasks;
        size_t totalTasks;
        char cpuName[128];
        float totalUsage;
        float perCore[32];
        size_t coreCount;
        size_t threadCount;
        double uptimeSeconds;
    } CpuStats;

    int get_cpu_stats(CpuStats *out); // neutral func
    void getProcStat(char *buffer, const int size);
    void calcTotalCpuUsage(char *prevBuffer, char *currentBuffer);
    void calcEveryCoreUsage(char *prevBuffer, char *currentBuffer);
    void getTotalCpuUsage(char *prevBuffer, char *currentBuffer);
    void getCpuStats(float *totalUsage, float *perCore, size_t totalUsageSize, size_t perCoreSize, char *prevBuffer, char *currentBuffer);  
    void CoreCount();
    void getCoreCount(size_t *coreCount, size_t coreCountSize);
    void CpuName();
    void getCpuName(char *cpuName, size_t cpuNameSize);
    void ThreadCount();
    void getThreadCount(size_t *threadCount, size_t threadCountSize);
    void getLoadavgData(float *loadAvg, size_t *ruinningTasks, size_t *totalTasks, size_t loadAvgSize, size_t runningTasksSize, size_t totalTasksSize);
    unsigned long long calcTotalCpuTck(char *prevBuffer, char *currentBuffer);
    unsigned long long getTotalCpuTicks();
    void LoadAvgData();
    void getUptimeSeconds(double *uptimeSeconds, size_t uptimeSecondsSize);
    void UptimeSeconds();
#ifdef __cplusplus
}
#endif

#endif