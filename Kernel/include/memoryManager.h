#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stdbool.h>
#include <stddef.h>

// #define BUDDY

typedef struct MemoryInfo
{
    size_t allocated;
    size_t free;
    size_t total;
} MemoryInfo;

int initMemoryManager(void *address, size_t size);
void *malloc(size_t size);
void free(void *ptr);
void getMemoryInfo(MemoryInfo *memory_info_ptr);

#endif
