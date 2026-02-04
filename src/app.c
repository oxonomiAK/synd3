#include "app.h" 

long long timeInMilliseconds(void) 
{
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}


static int updateAllowed = 1;
void appRun()
{
    d_arr processes;
    d_arr prevProcesses;
    d_arr currProcesses;
    SysStatistics Sys;
    ptParams ptPr;
    TUIManager wins;
    


    int running = 1;

    appInit(&wins, &Sys, &ptPr);
    
    char prevBuffer[CPU_STAT_BUFFER_SIZE];
    char currentBuffer[CPU_STAT_BUFFER_SIZE];

    getProcStat(prevBuffer, CPU_STAT_BUFFER_SIZE);
    memcpy(currentBuffer, prevBuffer, CPU_STAT_BUFFER_SIZE);

    size_t memTotal = 0;
    getMemTotal(&memTotal, sizeof(memTotal));
    Sys.memTotal = memTotal;

    unsigned long long prevTotalTicks = getTotalCpuTicks();
    unsigned long long currTotalTicks = prevTotalTicks;
    currProcesses = fetchProcesses();

    const int renderIntervalMs = 50;
    const int sampleIntervalMs = 1000;
    long long lastRenderTime = timeInMilliseconds();
    long long lastSampleTime = timeInMilliseconds();

    while(running)
    {

        running = handleMainInput(&ptPr.selectedColumn, &ptPr.selectedProcess, currProcesses, wins);
        long long timepPoint = timeInMilliseconds();
        

        if((timepPoint- lastSampleTime) >= sampleIntervalMs && updateAllowed)
        {
            prevProcesses = currProcesses;
            prevTotalTicks = currTotalTicks;
            memcpy(prevBuffer, currentBuffer, CPU_STAT_BUFFER_SIZE);


            getProcStat(currentBuffer, CPU_STAT_BUFFER_SIZE);
            getCpuStats(&Sys.CPUtotal, Sys.CPUpercore, sizeof(Sys.CPUtotal), sizeof(Sys.CPUpercore), prevBuffer, currentBuffer);
            
            getLoadavgData(Sys.loadAvg, &Sys.runningTasks, &Sys.totalTasks, sizeof(Sys.loadAvg), sizeof(Sys.runningTasks), sizeof(Sys.totalTasks));
            getUptimeSeconds(&Sys.uptimeSeconds, sizeof(Sys.uptimeSeconds));
            getMemUsedInKB(&Sys.memUsed, sizeof(Sys.memUsed));
            
            currProcesses = fetchProcesses();
            currTotalTicks = getTotalCpuTicks();
        
            Sys.percentMemUsed = (Sys.memUsed / Sys.memTotal) * 100.0f;
        
            
            for (int i = 0; i<=currProcesses.size; i++)
            {
                for (int j = 0; j<=prevProcesses.size; j++)
                {
                    if (currProcesses.process[i].pid == prevProcesses.process[j].pid)
                    {
                        unsigned long long deltaProc = currProcesses.process[i].process_ticks - prevProcesses.process[j].process_ticks;
                        unsigned long long deltaTotal = currTotalTicks - prevTotalTicks;
                        float usage = (deltaTotal > 0) ? (100.0f * (float)deltaProc / (float)deltaTotal) : 0.0f;
                        currProcesses.process[i].cpu_usage = usage;
                        break;
                    }
                }
            }
            lastSampleTime = timepPoint;
        }
        if((timepPoint- lastRenderTime)>= renderIntervalMs)
        {
            processes = currProcesses;
            sortColumns(&ptPr, &processes);
            uiDraw(&wins, &processes, Sys, &ptPr);


            lastRenderTime = timepPoint;
        }
        


        usleep(10000); //in microseconds, 10000 microsec = 10 millisec
    }


    appCleanup();
}



void appInit(TUIManager *wins, SysStatistics *Sys, ptParams *ptPr)
{
    initscr();			/* Start curses mode 		  */
    start_color();
    refresh();
    uiInit(wins);
    structInit(Sys, ptPr);
    noecho();
    cbreak();
    curs_set(0);
    timeout(100);
    
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    getThreadCount(&Sys->coreCount, sizeof(Sys->coreCount));
    getCpuName(Sys->cpuName, sizeof(Sys->cpuName));
    
}

void structInit(SysStatistics *Sys, ptParams *ptPr)
{
    int size = sizeof(Sys->CPUpercore) / sizeof(Sys->CPUpercore[0]);
    
    for(int i = 0; i <=size; i++)
    {
        Sys->CPUpercore[i] = 0;
    }
    Sys->CPUtotal = 0.0f;
    
    size = sizeof(Sys->loadAvg) / sizeof(Sys->loadAvg[0]);
    for(int i = 0; i <= size; i++)
    {
        Sys->loadAvg[i] = 0.0;
    }
    Sys->memTotal = 0.0f;
    Sys->memUsed = 0.0f;
    Sys->percentMemUsed = 0.0f;
    Sys->runningTasks = 0;
    Sys->totalTasks = 0;
    Sys->uptimeSeconds = 0;
    
    ptPr->scrollOffset = 0;
    ptPr->selectedColumn = 0;
    ptPr->selectedProcess = 0;
}
void appCleanup()
{

    endwin();			/* End curses mode		  */
}

void noUpdate()
{
    updateAllowed = 0;
}
void resumeUpdate()
{
    updateAllowed = 1;
}