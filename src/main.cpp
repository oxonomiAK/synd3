#include <iostream>
#include "ncurs.h"
#include "core/process.h"
#include <thread>
#include <chrono>

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

    return 0;
}