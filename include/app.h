#ifndef APP_H
#define APP_H

#include "core/input_handler.h"
#include <signal.h>
#include "core/cpu_Stats.h"
#include <string.h>
#include "core/mem_Info.h"
#include <sys/time.h>
#include "core/sort_col.h"




void appRun();
void appInit(TUIManager *wins, SysStatistics *Sys, ptParams *ptPr);
void structInit(SysStatistics *Sys, ptParams *ptPr);
void appCleanup();
void noUpdate();
void resumeUpdate();


#endif