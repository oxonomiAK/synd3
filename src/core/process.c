#include "core/process.h"

d_arr fetchProcesses()
{
    size_t MAX_PROCESSES = 1024;
    ProcessInfo buffer[MAX_PROCESSES];
    size_t count = get_process_list(buffer, MAX_PROCESSES);

    d_arr processes;
    createAr(&processes, count);

    for (size_t i = 0; i < count; ++i)
    {
        pushback(&buffer[i], &processes);
    }

    return processes;
}
