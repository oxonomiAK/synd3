#include <stdio.h>
#include <unistd.h> 
#include <stdbool.h>


static const size_t SIZE = 1024;
void getProcStat(char *buffer, const int size){
    FILE* file = fopen("/proc/stat", "r");
    if (file == NULL) {
        perror("Could not open stat file");
        return;
    }
    size_t prevret = fread(buffer, sizeof(char), size, file);
    if (prevret == 0) {
        perror("Could not read stat file");
        fclose(file);
        return;
    }
    fclose(file);
    
}


float calcTotalCpuUsage(char *prevBuffer, char *currentBuffer){
    unsigned long long iowait = 0, irq = 0, softirq = 0, steal = 0, guest = 0, guestnice = 0, user = 0, nice = 0, system = 0, idle = 0;
    unsigned long long previowait = 0, previrq = 0, prevsoftirq = 0, prevsteal = 0, prevguest = 0, prevguestnice = 0, prevuser = 0, prevnice = 0, prevsystem = 0, previdle = 0;

    sscanf(prevBuffer,
        "cpu  %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
        &prevuser, &prevnice, &prevsystem, &previdle, &previowait, &previrq, &prevsoftirq, &prevsteal, &prevguest, &prevguestnice);

    sscanf(currentBuffer,
            "cpu  %llu %llu %llu %llu %llu %llu %llu %llu %llu %llu",
            &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guestnice);

    unsigned long long PrevIdle = previdle + previowait;
    unsigned long long Idle = idle + iowait;
    unsigned long long PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    unsigned long long NonIdle = user + nice + system + irq + softirq + steal;
    unsigned long long PrevTotal = PrevIdle + PrevNonIdle;
    unsigned long long Total = Idle + NonIdle;

    unsigned long long deltaTotal = Total - PrevTotal;
    unsigned long long deltaIdle = Idle - PrevIdle;

    return ((float)(deltaTotal - deltaIdle) / (float)deltaTotal);

    
}



float getTotalCpuUsage() {
    char prevBuffer[SIZE];
    char currentBuffer[SIZE];
   

    getProcStat(prevBuffer, SIZE);

    sleep(1);

    getProcStat(currentBuffer, SIZE);

    float CPU_Percentage = calcTotalCpuUsage(prevBuffer, currentBuffer) * 100.0f;
    
    return CPU_Percentage;
}
