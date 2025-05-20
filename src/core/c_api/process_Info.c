#define _GNU_SOURCE
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "core/process_Info.h"
#include "core/cpu_Stats.h"

unsigned long long get_process_ticks(pid_t pid)
{
    char path[64];
    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        perror("Could not open stat file");
        return -1;
    }

    unsigned long long utime = 0, stime = 0;

    fscanf(file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %llu %llu", &utime, &stime);

    fclose(file);

    return utime + stime;
}

float calc_process_cpu_usage(unsigned long long prevProcessTicks, unsigned long long currProcessTicks, char *prevTotalBuff, char *currTotalBuff)
{
    unsigned long long deltaTotalTck = calcTotalCpuTck(prevTotalBuff, currTotalBuff);
    unsigned long long deltaProcTck = currProcessTicks - prevProcessTicks;

    return ((float)deltaProcTck / (float)deltaTotalTck) * 100.0f;
}

float get_process_cpu_usage(pid_t pid)
{
    char prevTotalBuffer[CPU_STAT_BUFFER_SIZE];
    char currTotalBuffer[CPU_STAT_BUFFER_SIZE];
    unsigned long long prevProcessTicks = get_process_ticks(pid);
    getProcStat(prevTotalBuffer, CPU_STAT_BUFFER_SIZE);

    sleep(1);

    unsigned long long currProcessTicks = get_process_ticks(pid);
    getProcStat(currTotalBuffer, CPU_STAT_BUFFER_SIZE);

    float processCpu_Percentage = calc_process_cpu_usage(prevProcessTicks, currProcessTicks, prevTotalBuffer, currTotalBuffer);

    return processCpu_Percentage;
}

void process_name_parsing(char *procNameBuffer, size_t procNameBufferSize, pid_t pid)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%d/cmdline", pid); // change with /proc/[PID]/cmdline and cut path(if possible)
    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        perror("Could not open stat file");
        return;
    }

    fgets(procNameBuffer, procNameBufferSize, file);
    procNameBuffer[strcspn(procNameBuffer, "\n")] = 0;
    fclose(file);
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

        // snprintf(buffer[count].name, sizeof(buffer[count].name), "process_%d", pid);

        process_name_parsing(buffer[count].name, sizeof(buffer[count].name), pid);

        buffer[count].pid = pid;
        buffer[count].cpu_usage = 0.0f;
        buffer[count].mem_usage = 0.0f;
        ++count;
    }

    closedir(proc);
    return count;
}
