#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "core/process_Info.h"
#include "core/cpu_Stats.h"
#include "core/mem_Info.h"

void get_process_ticks(pid_t pid, unsigned long long *buff)
{
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        // perror("Could not open stat file");
        return;
    }

    unsigned long long utime = 0, stime = 0;

    fscanf(file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %llu %llu", &utime, &stime);

    fclose(file);

    *buff = utime + stime;
}

void get_process_rss_kb(pid_t pid, unsigned long long *buff)
{
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/statm", pid);
    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        // perror("Could not open statm file");
        return;
    }
    unsigned long rss = 0;   // Resident Set Size in pages
    unsigned long dummy = 0; // Dummy variable to skip the first value (total program size)
    fscanf(file, "%lu %lu", &dummy, &rss);
    fclose(file);

    long page_size = sysconf(_SC_PAGE_SIZE) / 1024; // Get the system's page size in bytes

    *buff = rss * page_size;
}

// get_process_mem_usage - calculates the memory usage of a process in percentage
void get_process_mem_usage(pid_t pid, float *mem_usage)
{
    unsigned long long rss_kb = 0;
    size_t memTotal = 0;
    get_process_rss_kb(pid, &rss_kb);
    getMemTotal(&memTotal, sizeof(memTotal)); // Get total memory in KB

    if (memTotal == 0)
    {
        *mem_usage = 0.0f;
        return;
    }

    // Calculate memory usage as a percentage
    // rss_kb is in KB, memTotal is also in KB
    *mem_usage = ((float)rss_kb / (float)memTotal) * 100.0f;
}

int process_name_parsing(char *procNameBuffer, size_t procNameBufferSize, pid_t pid)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid); // change with /proc/[PID]/cmdline and cut path(if possible)
    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        // perror("Could not open stat file");
        return 0;
    }

    if (fgets(procNameBuffer, procNameBufferSize, file) == NULL)
    {
        fclose(file);
        return 0;
    }

    // procNameBuffer[strcspn(procNameBuffer, "\n")] = '\0';
    fclose(file);
    return 1;
}

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

        if (!process_name_parsing(buffer[count].name, sizeof(buffer[count].name), pid))
        {
            continue;
        }

        buffer[count].pid = pid;
        // buffer[count].cpu_usage = 0.0f;
        // buffer[count].mem_usage = 0.0f;
        get_process_mem_usage(pid, &buffer[count].mem_usage);
        get_process_ticks(pid, &buffer[count].process_ticks);
        // buffer[count].process_ticks = 0.0f;
        ++count;
    }

    closedir(proc);
    return count;
}
