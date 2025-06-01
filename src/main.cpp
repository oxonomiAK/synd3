#include <iostream>
#include <chrono>
#include "core/process.h"
#include <algorithm>
#include "MainWindow.h"


int main() {
    bool running = true;
    std::vector<Process> processes;
    MainWindow ui(processes);  
    ui.init();

    while (running) {

        if(!ui.getShowPopupWindow()) {
            processes = fetchProcesses();
        
            running = ui.handleInput(processes.size());

            int col = ui.getSelectedColumn();
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

        }
        ui.render(processes);
        std::this_thread::sleep_for(std::chrono::milliseconds(23));
    }
    ui.shutdown();
    endwin();
    return 0;
}    
