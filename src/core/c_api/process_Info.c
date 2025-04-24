#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "core/process_Info.h"

size_t get_process_list(ProcessInfo *buffer, size_t max)
{
    DIR *proc = opendir("/proc");
    if (!proc)
        return 0;

    size_t count = 0;
    struct dirent *entry;

    while ((entry = readdir(proc)) && count < max)
    {
        if (entry->d_type != DT_DIR)
            continue;
        int pid = atoi(entry->d_name);
        if (pid <= 0)
            continue;

        snprintf(buffer[count].name, sizeof(buffer[count].name), "process_%d", pid);
        buffer[count].pid = pid;
        buffer[count].cpu_usage = 0.0f;
        buffer[count].mem_usage = 0.0f;
        ++count;
    }

    closedir(proc);
    return count;
}
