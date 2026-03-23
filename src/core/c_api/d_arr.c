#include "core/d_arr.h"

// temp func name
void createAr(d_arr *ptr, int size)
{

    ptr->process = malloc(size * sizeof(ProcessInfo));

    if (ptr->process == NULL)
    {
        ptr->size = 0;
        ptr->last_index = 0;
        return;
    }
    ptr->size = size;
    ptr->last_index = 0;
    return;
}
// temp func name
void deleteAr(d_arr *ptr)
{
    if (ptr->process) // if ptr->process == NULL - we doesnt call free to prevent double free
    {
        free(ptr->process);
        ptr->process = NULL;
    }

    ptr->last_index = 0;
    ptr->size = 0;
}

void freeMemory(d_arr *prev, d_arr *curr)
{
    deleteAr(prev);
    deleteAr(curr);
}

// temp func name
void pushback(ProcessInfo *process, d_arr *ptr)
{

    if (ptr->last_index == ptr->size)
    {
        ProcessInfo *temp = ptr->process;
        ptr->size *= 2;

        ptr->process = realloc(ptr->process, sizeof(ProcessInfo) * ptr->size);
        if (!ptr->process)
        {
            ptr->process = temp;
            return;
        }
    }
    ptr->process[ptr->last_index] = *process;
    ptr->last_index++;
}

// temp solution (needs malloc's returned value check)   | Segmentation fault
void copyProcesses(d_arr *dest, d_arr *src)
{
    deleteAr(dest);
    createAr(dest, src->size);
    if (!dest->process)
        return;
    for (int i = 0; i < src->size; i++)
    {
        dest->process[i] = src->process[i];
    }
    dest->size = src->size;
    dest->last_index = src->last_index;
}