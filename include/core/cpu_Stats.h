#ifndef CPU_STATS_H
#define CPU_STATS_H

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        float total_usage;
        float per_core[32];
        int core_count;
    } CpuStats;

    int get_cpu_stats(CpuStats *out);

#ifdef __cplusplus
}
#endif

#endif