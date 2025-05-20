#include <iostream>
#include "ncurs.h"
#include "core/process.h"
#include <thread>
#include <chrono>
#include "core/cpu_Stats.h"
#include <unistd.h> 
int main()
{

    // NcursesUI ui;
    // ui.init();

    // while (true)
    // {
    //     std::vector<Process> processes = fetchProcesses();
    //     ui.render(processes);
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    // }

    // ui.shutdown();
    
    // char cpuName[128];
    float totalUsage;
    float perCore[32];
    // size_t coreCount;
    // size_t threadCount;
    // getCoreCount(&coreCount, sizeof(coreCount));
    getCpuStats(&totalUsage, perCore, sizeof(totalUsage), sizeof(perCore));
    while(true){
        getCpuStats(&totalUsage, perCore, sizeof (totalUsage), sizeof(perCore));
        printf("CPU: %f%% \n",totalUsage);
        for(int i=0; i<sysconf(_SC_NPROCESSORS_ONLN); i++){
            printf("Core:%d = %f%% \n",i,perCore[i]);
        }

    }
    return 0;
}