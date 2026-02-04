#include "core/d_arr.h"


void createAr(d_arr* ptr, int size){

    ptr->process = malloc(size * sizeof(ProcessInfo));

    if (ptr->process == NULL) {
        return ;
    }
    ptr->size = size;
    ptr->last_index = 0;
    return;
}

void pushback(ProcessInfo *process, d_arr* ptr){

    if(ptr->last_index == ptr->size){
        ProcessInfo *temp = ptr->process;
        ptr->size *= 2;
        
        ptr->process = realloc(ptr->process, sizeof(ProcessInfo) * ptr->size);
        if (!ptr->process) {
            ptr->process = temp;
            return;
        }
    }
    ptr->process[ptr->last_index] = *process;
    ptr->last_index++;

}