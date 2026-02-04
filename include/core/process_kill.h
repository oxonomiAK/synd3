#ifndef PROCESS_KILL_H
#define PROCESS_KILL_H

#include "core/d_arr.h"
#include <signal.h>
#include <errno.h>

int killProcess(d_arr processes, int selectedProcess);

#endif