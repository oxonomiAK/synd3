#include "core/process_Info.h"
#include "core/process.h"
#include <vector>

std::vector<Process> fetchProcesses()
{
    constexpr size_t MAX_PROCESSES = 1024;
    ProcessInfo buffer[MAX_PROCESSES];

    size_t count = get_process_list(buffer, MAX_PROCESSES);

    std::vector<Process> processes;
    processes.reserve(count);

    for (size_t i = 0; i < count; ++i)
    {
        processes.emplace_back(buffer[i]);
    }

    return processes;
}