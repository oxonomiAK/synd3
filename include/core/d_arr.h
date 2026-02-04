#ifndef D_ARR_H
#define D_ARR_H

#include <stdio.h>
#include <stdlib.h>
#include "core/process_Info.h"

    typedef struct 
    {
        int size;
        ProcessInfo* process;
        int last_index;
        
    }d_arr;

    void createAr(d_arr* ptr, int size);
    void pushback(ProcessInfo *process, d_arr* ptr);

#endif