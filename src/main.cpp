

#include <iostream>
#include <thread>
#include <chrono>
#include "core/process.h"
#include <algorithm>
#include <atomic>
#include "MainWindow.h"

std::atomic<bool> running(true);
std::mutex mtx, mtx1;
std::vector<Process> processes;


void initHandling(MainWindow& ui){   
    std::lock_guard<std::mutex> lock(mtx);
    while (running.load()) {
        bool stillRunning = ui.handleInput(processes.size());
        if (!stillRunning) {
            running.store(false);
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    }

}

int main() {
    std::lock_guard<std::mutex> lock(mtx1);
    MainWindow ui(processes);  
    ui.init();



    std::thread inputHandle(initHandling, std::ref(ui));
    while (running.load()) {

        if(!ui.getShowPopupWindow()) {
            processes = fetchProcesses();
        
        
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
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    ui.shutdown();
    inputHandle.join();
    return 0;
}    
