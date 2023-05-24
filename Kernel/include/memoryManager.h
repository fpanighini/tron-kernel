#ifndef MEM_MANAGER_H
#define MEM_MANAGER_H

#include <stddef.h>

int initMemoryManager(void *address, size_t size);
void * malloc(size_t size);
void free(void *ptr);

#endif
