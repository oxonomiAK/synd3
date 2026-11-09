#ifndef CONTAINER_INFO_H
#define CONTAINER_INFO_H

#include "core/process_Info.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>


void get_container_info(pid_t PID, char *buffer);
int check_NSpid(pid_t PID);
int check_cgroup(pid_t PID);

typedef enum{
    NONE,
    DOCKER,
    KUBERNETES,
    OTHER


}containerType;

#endif