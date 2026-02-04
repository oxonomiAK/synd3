#include "core/sort_col.h"

int compPid(const void* a, const void* b) {
  	return (((ProcessInfo*)b)->pid > ((ProcessInfo*)a)->pid) ? -1 : (((ProcessInfo*)b)->pid < ((ProcessInfo*)a)->pid);
}

int compMemUsage(const void* a, const void* b) {
    float fa = ((ProcessInfo*)a)->mem_usage;
    float fb = ((ProcessInfo*)b)->mem_usage;
  	return (fa > fb) ? -1 : (fa < fb);
}

int compCpuUsage(const void* a, const void* b) {
    float fa = ((ProcessInfo*)a)->cpu_usage;
    float fb = ((ProcessInfo*)b)->cpu_usage;
  	return (fa > fb) ? -1 : (fa < fb);
}

int compName(const void* a, const void* b) {
  	return strcmp(((ProcessInfo*)a)->name, ((ProcessInfo*)b)->name);
}


void sortColumns(ptParams *ptPr, d_arr *procesess){
    
    switch(ptPr->selectedColumn){
        case 0:
            qsort(procesess->process, procesess->size, sizeof(ProcessInfo), compPid);
        break;
        case 1: 
            qsort(procesess->process, procesess->size, sizeof(ProcessInfo), compName);
        break;
        case 2:
            qsort(procesess->process, procesess->size, sizeof(ProcessInfo), compCpuUsage);
        break;
        case 3:
            qsort(procesess->process, procesess->size, sizeof(ProcessInfo), compMemUsage);
        break;
    }
    
}