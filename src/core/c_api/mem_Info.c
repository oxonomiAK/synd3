#include <stdio.h>
#include <unistd.h> 
#include <stdbool.h>
// #include"core/mem_Info.h"
#include "/home/arlive/glance/include/core/mem_Info.h"
#include <string.h>
#include <stdlib.h>

MemInfo meminfo;


void getMemTotal(size_t *memTotal, size_t memTotaleSize){
    MemTotal();
    memcpy(memTotal, &meminfo.memTotal, memTotaleSize);
}
void getMemFree(size_t *memFree, size_t memFreeSize){
    MemFree();
    memcpy(memFree, &meminfo.memFree, memFreeSize);
}
void getMemTotalUsed(size_t *memTotalUsed, size_t memTotalUsedSize){
    MemTotalUsed();
    memcpy(memTotalUsed, &meminfo.memTotalUsed, memTotalUsedSize);
}
void getBuffers(size_t *buffers, size_t buffersSize){
    Buffers();
    memcpy(buffers, &meminfo.buffers, buffersSize);
}
void getCached(size_t *cached, size_t cachedSize){
    Cached();
    memcpy(cached, &meminfo.cached, cachedSize);
}
void getSReclaimable(size_t *sReclaimable, size_t sReclaimableSize){
    SReclaimable();
    memcpy(sReclaimable, &meminfo.sReclaimable, sReclaimableSize);
}
void getShmem(size_t *shmem, size_t shmemSize){
    Shmem();
    memcpy(shmem, &meminfo.shmem, shmemSize);
}
void getCachedMem(size_t *cachedMem, size_t cachedMemSize){
    CachedMem();
    memcpy(cachedMem, &meminfo.cachedMem, cachedMemSize);
}
void getMemUsedInMB(size_t *memUsed, size_t memUsedSize){
    MemUsed();
    memcpy(memUsed, &meminfo.memUsedInMB, memUsedSize);
}
void MemTotal(){
    FILE* file = fopen("/proc/meminfo", "r");
    char buffer[MEM_INFO_BUFFER_SIZE];
    if (file == NULL) {
        perror("Could not open file");
        return;
    }

    fgets(buffer, MEM_INFO_BUFFER_SIZE, file);
    
    char* temp = buffer+9;
    meminfo.memTotal = atoi(temp);
    fclose(file);
}

void MemFree(){
    FILE* file = fopen("/proc/meminfo", "r");
    char buffer[MEM_INFO_BUFFER_SIZE];
    if (file == NULL) {
        perror("Could not open file");
        return;
    }
    for(size_t i = 0; i<=1; i++){
        fgets(buffer, MEM_INFO_BUFFER_SIZE, file);
    }
    char* temp = buffer+9;
    meminfo.memFree = atoi(temp);
    fclose(file);
}

void MemTotalUsed(){
    size_t MemTotal, MemFree;
    getMemTotal(&MemTotal, sizeof(MemTotal));
    getMemFree(&MemFree, sizeof(MemFree));
    meminfo.memTotalUsed = MemTotal - MemFree;
}

void Buffers(){
    FILE* file = fopen("/proc/meminfo", "r");
    char buffer[MEM_INFO_BUFFER_SIZE];
    if (file == NULL) {
        perror("Could not open file");
        return;
    }
    for(size_t i = 0; i<=3; i++){
        fgets(buffer, MEM_INFO_BUFFER_SIZE, file);
    }
    char* temp = buffer+8;
    meminfo.buffers = atoi(temp);
    fclose(file);
}

void Cached(){
    FILE* file = fopen("/proc/meminfo", "r");
    char buffer[MEM_INFO_BUFFER_SIZE];
    if (file == NULL) {
        perror("Could not open file");
        return;
    }
    for(size_t i = 0; i<=4; i++){
        fgets(buffer, MEM_INFO_BUFFER_SIZE, file);
    }
    char* temp = buffer+7;
    meminfo.cached = atoi(temp);
    fclose(file);
}

void SReclaimable(){
    FILE* file = fopen("/proc/meminfo", "r");
    char buffer[MEM_INFO_BUFFER_SIZE];
    if (file == NULL) {
        perror("Could not open file");
        return;
    }
    for(size_t i = 0; i<=23; i++){
        fgets(buffer, MEM_INFO_BUFFER_SIZE, file);
    }
    char* temp = buffer+13;
    meminfo.sReclaimable = atoi(temp);
    fclose(file);
}

void Shmem(){
    FILE* file = fopen("/proc/meminfo", "r");
    char buffer[MEM_INFO_BUFFER_SIZE];
    if (file == NULL) {
        perror("Could not open file");
        return;
    }
    for(size_t i = 0; i<=20; i++){
        fgets(buffer, MEM_INFO_BUFFER_SIZE, file);
    }
    char* temp = buffer+6;
    meminfo.shmem = atoi(temp);
    fclose(file);
}
void CachedMem(){
    size_t cached,  sReclaimable, shmem;
    getCached(&cached, sizeof(cached));
    getSReclaimable(&sReclaimable, sizeof(sReclaimable));
    getShmem(&shmem, sizeof(shmem));
    meminfo.cachedMem = cached + sReclaimable - shmem;
}
void MemUsed(){
    size_t memTotalUsed, buffers, cachedMem;
    getMemTotalUsed(&memTotalUsed, sizeof(memTotalUsed));
    getBuffers(&buffers, sizeof(buffers));
    getCachedMem(&cachedMem, sizeof(cachedMem));
    meminfo.memUsedInMB = (memTotalUsed-(buffers+cachedMem))/1024;
}
int main(){
    size_t MemTotal;
    while(true){
        getMemUsedInMB(&MemTotal, sizeof(MemTotal)); 
        printf("%ld\n", MemTotal);
        sleep(1);
    }

    return 0;
}