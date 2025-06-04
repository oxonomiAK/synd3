#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include "core/cpu_Stats.h"
#include <string.h>
#include <stdlib.h>

CpuStats cpustats;
void getProcStat(char *buffer, const int size)
{
    FILE *file = fopen("/proc/stat", "r");
    if (file == NULL)
    {
        perror("Could not open stat file");
        return;
    }
    size_t ret = fread(buffer, sizeof(char), size, file);
    if (ret == 0)
    {
        perror("Could not read stat file");
        fclose(file);
        return;
    }
    fclose(file);
}

unsigned long long calcTotalCpuTck(char *prevBuffer, char *currentBuffer)
{
    unsigned long long iowait = 0, irq = 0, softirq = 0, steal = 0, guest = 0, guestnice = 0, user = 0, nice = 0, system = 0, idle = 0;
    unsigned long long previowait = 0, previrq = 0, prevsoftirq = 0, prevsteal = 0, prevguest = 0, prevguestnice = 0, prevuser = 0, prevnice = 0, prevsystem = 0, previdle = 0;

    sscanf(prevBuffer,
           "%*c %*c %*c %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
           &prevuser, &prevnice, &prevsystem, &previdle, &previowait, &previrq, &prevsoftirq, &prevsteal, &prevguest, &prevguestnice);

    sscanf(currentBuffer,
           "%*c %*c %*c %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guestnice);

    unsigned long long PrevTotalTck = previowait + previrq + prevsoftirq + prevsteal + prevguest + prevguestnice + prevuser + prevnice + prevsystem + previdle;
    unsigned long long TotalTck = iowait + irq + softirq + steal + guest + guestnice + user + nice + system + idle;

    unsigned long long deltaTotalTck = TotalTck - PrevTotalTck;

    return deltaTotalTck;
}

void calcTotalCpuUsage(char *prevBuffer, char *currentBuffer)
{
    unsigned long long iowait = 0, irq = 0, softirq = 0, steal = 0, guest = 0, guestnice = 0, user = 0, nice = 0, system = 0, idle = 0;
    unsigned long long previowait = 0, previrq = 0, prevsoftirq = 0, prevsteal = 0, prevguest = 0, prevguestnice = 0, prevuser = 0, prevnice = 0, prevsystem = 0, previdle = 0;
    sscanf(prevBuffer,
           "%*c %*c %*c %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
           &prevuser, &prevnice, &prevsystem, &previdle, &previowait, &previrq, &prevsoftirq, &prevsteal, &prevguest, &prevguestnice);
    sscanf(currentBuffer,
           "%*c %*c %*c %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guestnice);
    unsigned long long PrevIdle = previdle + previowait;
    unsigned long long Idle = idle + iowait;
    unsigned long long PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    unsigned long long NonIdle = user + nice + system + irq + softirq + steal;
    unsigned long long PrevTotal = PrevIdle + PrevNonIdle;
    unsigned long long Total = Idle + NonIdle;
    unsigned long long deltaTotal = Total - PrevTotal;
    unsigned long long deltaIdle = Idle - PrevIdle;

    cpustats.totalUsage = (float)(deltaTotal - deltaIdle) / (float)deltaTotal * 100.0f;
}

void calcEveryCoreUsage(char *prevBuffer, char *currentBuffer)
{

    char *pBuffer = prevBuffer;
    char *cBuffer = currentBuffer;
    size_t count = 0;
    size_t threads;
    getThreadCount(&threads, sizeof(threads));
    pBuffer = prevBuffer + count + strcspn(prevBuffer, "\n") + 2;
    cBuffer = currentBuffer + count + strcspn(currentBuffer, "\n") + 2;
    count = count + strcspn(pBuffer, "\n") + 2;
    for (size_t i = 0; i <= threads; i++)
    {

        unsigned long long iowait = 0, irq = 0, softirq = 0, steal = 0, guest = 0, guestnice = 0, user = 0, nice = 0, system = 0, idle = 0;
        unsigned long long previowait = 0, previrq = 0, prevsoftirq = 0, prevsteal = 0, prevguest = 0, prevguestnice = 0, prevuser = 0, prevnice = 0, prevsystem = 0, previdle = 0;

        sscanf(pBuffer,
               "%*c %*c %*c %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
               &prevuser, &prevnice, &prevsystem, &previdle, &previowait, &previrq, &prevsoftirq, &prevsteal, &prevguest, &prevguestnice);

        sscanf(cBuffer,
               "%*c %*c %*c %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
               &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guestnice);

        unsigned long long PrevIdle = previdle + previowait;
        unsigned long long Idle = idle + iowait;
        unsigned long long PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
        unsigned long long NonIdle = user + nice + system + irq + softirq + steal;
        unsigned long long PrevTotal = PrevIdle + PrevNonIdle;
        unsigned long long Total = Idle + NonIdle;

        unsigned long long deltaTotal = Total - PrevTotal;
        unsigned long long deltaIdle = Idle - PrevIdle;
        if (deltaIdle > deltaTotal)
        {
            if (i >= 9)
            {
                count++;
            }
            pBuffer = prevBuffer + count + strcspn(prevBuffer, "\n") + 2;
            cBuffer = currentBuffer + count + strcspn(currentBuffer, "\n") + 2;
            count = count + strcspn(pBuffer, "\n") + 2;
            continue;
        }

        cpustats.perCore[i] = (float)(deltaTotal - deltaIdle) / (float)deltaTotal * 100.0f;
        if (i >= 9)
        {
            count++;
        }
        pBuffer = prevBuffer + count + strcspn(prevBuffer, "\n") + 2;
        cBuffer = currentBuffer + count + strcspn(currentBuffer, "\n") + 2;
        count = count + strcspn(pBuffer, "\n") + 2;
    }
}
void getCpuStats(float *totalUsage, float *perCore, size_t totalUsageSize, size_t perCoreSize)
{
    getTotalCpuUsage();
    memcpy(totalUsage, &cpustats.totalUsage, totalUsageSize);
    memcpy(perCore, &cpustats.perCore, perCoreSize);
}

void getCoreCount(size_t *coreCount, size_t coreCountSize)
{
    CoreCount();
    memcpy(coreCount, &cpustats.coreCount, coreCountSize);
}

void getCpuName(char *cpuName, size_t cpuNameSize)
{
    CpuName();
    memcpy(cpuName, &cpustats.cpuName, cpuNameSize);
}
void getThreadCount(size_t *threadCount, size_t threadCountSize)
{
    ThreadCount();
    memcpy(threadCount, &cpustats.threadCount, threadCountSize);
}

void CoreCount()
{
    FILE *file = fopen("/proc/cpuinfo", "r");
    char buffer[CPU_STAT_BUFFER_SIZE];
    if (file == NULL)
    {
        // perror("Could not open file");
        return;
    }
    for (size_t i = 0; i <= 12; i++)
    {
        fgets(buffer, CPU_STAT_BUFFER_SIZE, file);
    }
    char *coreCount = buffer + 12;
    cpustats.coreCount = atoi(coreCount);
    fclose(file);
}

void CpuName()
{
    FILE *file = fopen("/proc/cpuinfo", "r");
    char buffer[CPU_STAT_BUFFER_SIZE];
    if (file == NULL)
    {
        // perror("Could not open file");
        return;
    }
    for (size_t i = 0; i <= 4; i++)
    {
        fgets(buffer, CPU_STAT_BUFFER_SIZE, file);
    }
    char *cpuName = buffer + 13;

    strncpy(cpustats.cpuName, cpuName, sizeof(cpustats.cpuName));
    fclose(file);
}

void ThreadCount()
{
    cpustats.threadCount = sysconf(_SC_NPROCESSORS_ONLN);
}

void getTotalCpuUsage()
{
    char prevBuffer[CPU_STAT_BUFFER_SIZE];
    char currentBuffer[CPU_STAT_BUFFER_SIZE];

    getProcStat(prevBuffer, CPU_STAT_BUFFER_SIZE);

    sleep(1);

    getProcStat(currentBuffer, CPU_STAT_BUFFER_SIZE);

    calcTotalCpuUsage(prevBuffer, currentBuffer);
    calcEveryCoreUsage(prevBuffer, currentBuffer);
}
