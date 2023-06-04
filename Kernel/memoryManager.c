#include "memoryManager.h"

#ifndef BUDDY

typedef struct BlockData {
    size_t size;
    struct BlockData *next;
} BlockData;

static BlockData *free_list = NULL;
static BlockData *alloc_list = NULL;

static MemoryInfo memory_info;

int initMemoryManager(void *address, size_t size) {
    // Misaligned address
    if ((size_t)address % sizeof(size_t) != 0)
        return 1;

    // Misaligned size
    if (size % sizeof(size_t) != 0)
        return 2;

    // Not enough memory
    if (size < sizeof(BlockData) + sizeof(size_t))
        return 3;

    BlockData *initial_block = (BlockData *)address;

    initial_block->size = size - sizeof(BlockData);
    initial_block->next = NULL;

    free_list = initial_block;

    memory_info.allocated = sizeof(BlockData);
    memory_info.free = size - sizeof(BlockData);
    memory_info.total = size;

    return 0;
}

void *malloc(size_t size) {
    if (size == 0)
        return NULL;

    // Align size
    if (size % sizeof(size_t) > 0)
        size += sizeof(size_t) - size % sizeof(size_t);

    // Search first fit
    BlockData *block = free_list;
    BlockData *previous = NULL;
    while (block != NULL && block->size < size) {
        previous = block;
        block = block->next;
    }

    // Not found
    if (block == NULL)
        return NULL;

    // If it's bigger than necessary, split
    if (block->size >= size + sizeof(BlockData) + sizeof(size_t)) {
        char *free_space = (char *)(block + 1);
        BlockData *new_block = (BlockData *)(free_space + size);

        new_block->size = block->size - sizeof(BlockData) - size;
        new_block->next = block->next;

        block->next = new_block;
        block->size = size;

        memory_info.allocated += sizeof(BlockData);
        memory_info.free -= sizeof(BlockData);
    }

    // Extract block from free list
    if (previous != NULL) {
        previous->next = block->next;
    } else {
        free_list = block->next;
    }
    memory_info.free -= block->size;

    // Add block to alloc list
    block->next = alloc_list;
    alloc_list = block;
    memory_info.allocated += block->size;

    return (void *)(block + 1);
}

int isContiguous(BlockData *left_block, BlockData *right_block) {
    char *left_data = (char *)(left_block + 1);
    BlockData *next_block = (BlockData *)(left_data + left_block->size);
    return next_block == right_block;
}

void free(void *ptr) {
    // Is it a valid allocated block?
    BlockData *block = (BlockData *)ptr - 1;

    // Search for that block
    BlockData *next = alloc_list;
    BlockData *previous = NULL;
    while (next != NULL && next != block) {
        previous = next;
        next = next->next;
    }

    // Not found, it's an imposter!
    if (next == NULL)
        return;

    // Extract block from alloc list
    if (previous != NULL) {
        previous->next = next->next;
    } else {
        alloc_list = next->next;
    }
    memory_info.allocated -= block->size;

    // Search for its place in the free list, order by address
    next = free_list;
    previous = NULL;
    while (next != NULL && next < block) {
        previous = next;
        next = next->next;
    }

    // Add block to free list, in the corresponding position
    if (previous == NULL) {
        block->next = free_list;
        free_list = block;
    } else {
        block->next = next;
        previous->next = block;
    }
    memory_info.free += block->size;

    // Check contiguity with next free block and merge
    if (next != NULL && isContiguous(block, next)) {
        block->next = next->next;
        block->size += sizeof(BlockData) + next->size;
        memory_info.allocated -= sizeof(BlockData);
        memory_info.free += sizeof(BlockData);
    }

    // Check contiguity with previous free block and merge
    if (previous != NULL && isContiguous(previous, block)) {
        previous->next = block->next;
        previous->size += sizeof(BlockData) + block->size;
        memory_info.allocated -= sizeof(BlockData);
        memory_info.free += sizeof(BlockData);
    }
}

void getMemoryInfo(MemoryInfo *memory_info_ptr) {
    *memory_info_ptr = memory_info;
}

#endif
