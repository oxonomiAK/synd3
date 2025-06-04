// #ifndef PROCESS_H
// #define PROCESS_H

// #include <string>
// #include "process_Info.h"
// #include <vector>

// class Process
// {
// public:
//     explicit Process(const ProcessInfo &info)
//         : pid(info.pid), name(info.name), cpu(info.cpu_usage), mem(info.mem_usage) {}

//     int getPid() const { return pid; }
//     std::string getName() const { return name; }
//     float getCpuUsage() const { return cpu; }
//     float getMemUsage() const { return mem; }

// private:
//     int pid;
//     std::string name;
//     float cpu;
//     float mem;
// };

// std::vector<Process> fetchProcesses();

// #endif

#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "process_Info.h"
#include <vector>

class Process
{
public:
    Process() : pid(0), name(""), cpu(0.0f), mem(0.0f), ticks(0) {} // constructor for default initialization

    explicit Process(const ProcessInfo &info)
        : pid(info.pid), name(info.name), cpu(0.0f), mem(info.mem_usage), ticks(info.process_ticks) {}

    int getPid() const { return pid; }
    std::string getName() const { return name; }
    float getCpuUsage() const { return cpu; }
    float getMemUsage() const { return mem; }
    unsigned long long getTicks() const { return ticks; }
    void setProcessCpuUsage(float cpuUsage) { cpu = cpuUsage; }

private:
    int pid;
    std::string name;
    float cpu;
    float mem;
    unsigned long long ticks;
};

std::vector<Process> fetchProcesses();

#endif