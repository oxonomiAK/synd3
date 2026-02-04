#include "core/process_kill.h"



int killProcess(d_arr processes, int selectedProcess)
{
    if(!kill(processes.process[selectedProcess].pid, SIGTERM))
    {
        return 1;
    }else{
        int err = errno;    
        if (err == EPERM)
        {

            return 0;
        }
    }

    return 1;

}