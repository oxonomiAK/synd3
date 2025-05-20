#ifndef CPU_STATS_H
#define CPU_STATS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define CPU_STAT_BUFFER_SIZE 1024

    typedef struct
    {
        float total_usage;
        float per_core[32];
        int core_count;
    } CpuStats;

    int get_cpu_stats(CpuStats *out);
    void getProcStat(char *buffer, const int size);
    float calcTotalCpuUsage(char *prevBuffer, char *currentBuffer);
    float getTotalCpuUsage();
    unsigned long long getTotalCpuTck(char *prevBuffer, char *currentBuffer);
    unsigned long long calcTotalCpuTck(char *prevBuffer, char *currentBuffer);

#ifdef __cplusplus
}
#endif

#endif