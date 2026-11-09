#include "core/container_info.h"


void get_container_info(pid_t PID, char *buffer){


    char *contNames[] ={" ", "Docker", "Kubernetes"};
    
    if(check_NSpid(PID) < 2){
        strcpy(buffer, contNames[0]);
        return;
    }
    int contType = check_cgroup(PID);
    strcpy(buffer, contNames[contType]);


}



int check_cgroup(pid_t PID){
    char *contNameHints[] = {"docker", "kube"};
    char path[64] = {0};
    int contType = NONE;
    snprintf(path, sizeof(path), "/proc/%d/cgroup", PID);
    
    FILE *file = fopen(path, "r");
    if (!file)
    {
        // perror("Could not open file");
        return 0;
    }

    char buff[256];
    while(fgets(buff, sizeof(buff), file) && !contType){

        for(int i = 0; i<(int)(sizeof(contNameHints)/sizeof(char*)); i++){
            if(strstr(buff, contNameHints[i])){
                switch (i+1)
                {
                case DOCKER:
                    contType = DOCKER;
                    break;
                case KUBERNETES:
                    contType = KUBERNETES;
                    break;

                default:
                    contType = NONE;
                    break;
                }

            }
        
        }
        
    }
    fclose(file);
    return contType;
}


int check_NSpid(pid_t PID){

    

    char path[64] = {0};
    snprintf(path, sizeof(path), "/proc/%d/status", PID);
    char buff[128] = {0};
    int NSpidCount = 0;
    FILE *file = fopen(path, "r");
    if (!file)
    {
        // perror("Could not open file");
        return 0;
    }
    
    while(fgets(buff, sizeof(buff), file)){
        char *ptr = buff;
        if(strstr(buff, "NSpid:")){

            while(*ptr != '\n' && *ptr){
                ptr++;

                while(!isdigit(*ptr)){
                    ptr++;
                }
                NSpidCount++;
                while(*ptr != '\t' && *ptr){
                    ptr++;
                }
            }

            break;
        }

    }

    fclose(file);
    return NSpidCount;
    
}