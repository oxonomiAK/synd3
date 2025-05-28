#ifndef MEM_INFO_H
#define MEM_INFO_H

#ifdef __cplusplus
extern "C"
{
#endif
#define MEM_INFO_BUFFER_SIZE 1024
#include <stddef.h>
    typedef struct
    {
        size_t memTotal;
        size_t memFree;
        size_t memTotalUsed;
        size_t buffers;
        size_t cached;
        size_t sReclaimable;
        size_t shmem;
        size_t cachedMem;
        size_t memUsedInMB;
    } MemInfo;

    void MemTotal();
    void getMemTotal(size_t *memTotal, size_t memTotaleSize);
    void MemFree();
    void getMemFree(size_t *memFree, size_t memFreeSize);
    void MemTotalUsed();
    void getMemTotalUsed(size_t *memTotalUsed, size_t memTotalUsedSize);
    void Buffers();
    void getBuffers(size_t *buffers, size_t buffersSize);
    void Cached();
    void getCached(size_t *cached, size_t cachedSize);
    void SReclaimable();
    void getSReclaimable(size_t *sReclaimable, size_t sReclaimableSize);
    void Shmem();
    void getShmem(size_t *shmem, size_t shmemSize);
    void CachedMem();
    void getCachedMem(size_t *cachedMem, size_t cachedMemSize);
    void MemUsed();
    void getMemUsedInMB(size_t *memUsed, size_t memUsedSize);
#ifdef __cplusplus
}
#endif

#endif