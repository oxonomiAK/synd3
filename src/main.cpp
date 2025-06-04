#include <iostream>
#include <chrono>
#include <thread>
#include "core/process.h"
#include <algorithm>
#include "MainWindow.h"

int main()
{
    bool running = true;
    std::vector<Process> processes;
    std::vector<Process> prevProcesses;
    std::vector<Process> currProcesses;

    unsigned long long prevTotalTicks = getTotalCpuTicks();
    unsigned long long currTotalTicks = prevTotalTicks;

    MainWindow ui(processes);
    ui.init();

    std::chrono::steady_clock::time_point lastRenderTime = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point lastSampleTime = std::chrono::steady_clock::now();

    const int renderIntervalMs = 50;
    const int sampleIntervalMs = 1000;

    while (running)
    {

        running = ui.handleInput(processes.size());

        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSampleTime).count() >= sampleIntervalMs)
        {
            prevProcesses = currProcesses;
            prevTotalTicks = currTotalTicks;

            currProcesses = fetchProcesses();
            currTotalTicks = getTotalCpuTicks();

            for (auto &proc : currProcesses)
            {
                for (auto &prev : prevProcesses)
                {
                    if (proc.getPid() == prev.getPid())
                    {
                        unsigned long long deltaProc = proc.getTicks() - prev.getTicks();
                        unsigned long long deltaTotal = currTotalTicks - prevTotalTicks;
                        float usage = (deltaTotal > 0) ? (100.0f * (float)deltaProc / (float)deltaTotal) : 0.0f;
                        proc.setProcessCpuUsage(usage);
                        break;
                    }
                }
            }

            lastSampleTime = now;
        }

        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRenderTime).count() >= renderIntervalMs)
        {
            if (!ui.getShowPopupWindow())
            {
                processes = currProcesses;

                int col = ui.getSelectedColumn();
                std::sort(processes.begin(), processes.end(), [col](const Process &a, const Process &b)
                          {
                    switch (col) {
                        case 0: return a.getPid() < b.getPid();
                        case 1: return a.getName() < b.getName();
                        case 2: return a.getCpuUsage() > b.getCpuUsage();
                        case 3: return a.getMemUsage() > b.getMemUsage();
                        default: return a.getPid() < b.getPid();
                    } });

                if (processes.size() > 1024)
                {
                    processes.resize(1024);
                }
            }

            ui.render(processes);
            lastRenderTime = now;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    ui.shutdown();

    endwin();
    return 0;
}
