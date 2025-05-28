// #include <iostream>
// #include "ncurs.h"
// #include "core/process.h"
// #include <thread>
// #include <chrono>
// #include "core/cpu_Stats.h"
// #include <unistd.h>
// int main()
// {

//     NcursesUI ui;
//     ui.init();

//     while (true)
//     {
//         std::vector<Process> processes = fetchProcesses();
//         ui.render(processes);
//         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//     }

//     ui.shutdown();

//     // ++++++++++++++++++++++++++++++++++++++++++++ sergo code()

//     // float totalUsage;
//     // float perCore[32];

//     // getCpuStats(&totalUsage, perCore, sizeof(totalUsage), sizeof(perCore));
//     // while(true){
//     //     getCpuStats(&totalUsage, perCore, sizeof (totalUsage), sizeof(perCore));
//     //     printf("CPU: %f%% \n",totalUsage);
//     //     for(int i=0; i<sysconf(_SC_NPROCESSORS_ONLN); i++){
//     //         printf("Core:%d = %f%% \n",i,perCore[i]);
//     //     }

//     // }
//     return 0;
// }

#include <iostream>
#include <thread>
#include <chrono>
#include "ncurs.h"
#include "core/process.h"
#include <algorithm>

int main() {
    NcursesUI ui;
    ui.init();

    bool running = true;
    std::vector<Process> processes;

    std::thread refreshScreen(refresh_screen);

    while (running) {
        processes = fetchProcesses();

        int col = ui.getSortColumn();
        std::sort(processes.begin(), processes.end(), [col](const Process& a, const Process& b){
            switch (col) {
                case 0: return a.getPid() < b.getPid();
                case 1: return a.getName() < b.getName();
                case 2: return a.getCpuUsage() > b.getCpuUsage();
                case 3: return a.getMemUsage() > b.getMemUsage();
                default: return a.getPid() < b.getPid();
            }
        });

        if (processes.size() > 1024) processes.resize(1024);

        ui.render(processes);

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        running = ui.handleInput(processes.size());
    }

    ui.shutdown();
    refreshScreen.();
    return 0;
}