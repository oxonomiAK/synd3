#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>    // size_t
#include <stdint.h>    // uint64_t
#include <sys/types.h> // pid_t

    typedef struct
    {
        int pid;
        char name[256];
        float cpu_usage;
        float mem_usage;
    } ProcessInfo;

    size_t get_process_list(ProcessInfo *buffer, size_t max);
    float calc_process_cpu_usage(unsigned long long prevProcessTicks, unsigned long long currProcessTicks, char *prevTotalBuff, char *currTotalBuff);
    unsigned long long get_process_ticks(pid_t pid);
    float get_process_cpu_usage(pid_t pid);

#ifdef __cplusplus
}
#endif

#endif