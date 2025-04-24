#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h> // size_t
#include <stdint.h> // uint64_t

    typedef struct
    {
        int pid;
        char name[256];
        float cpu_usage;
        float mem_usage;
    } ProcessInfo;

    size_t get_process_list(ProcessInfo *buffer, size_t max);

#ifdef __cplusplus
}
#endif

#endif