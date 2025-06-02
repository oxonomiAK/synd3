#include <iostream>
#include "ncurs.h"
#include "core/process.h"
#include <thread>
#include <chrono>
#include "core/cpu_Stats.h"
#include "core/mem_Info.h"
#include <unistd.h>
int main()
{

    NcursesUI ui;
    ui.init();

    while (true)
    {
        std::vector<Process> processes = fetchProcesses();
        ui.render(processes);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    ui.shutdown();

    // ++++++++++++++++++++++++++++++++++++++++++++ sergo code()

    // float totalUsage;
    // float perCore[32];

    // getCpuStats(&totalUsage, perCore, sizeof(totalUsage), sizeof(perCore));
    // while(true){
    //     getCpuStats(&totalUsage, perCore, sizeof (totalUsage), sizeof(perCore));
    //     printf("CPU: %f%% \n",totalUsage);
    //     for(int i=0; i<sysconf(_SC_NPROCESSORS_ONLN); i++){
    //         printf("Core:%d = %f%% \n",i,perCore[i]);
    //     }

    // }

    // size_t MemTotal;
    // while (true)
    // {
    //     getMemUsedInMB(&MemTotal, sizeof(MemTotal));
    //     printf("%ld\n", MemTotal);
    //     sleep(1);
    // }
    return 0;
}