#include "memoryManager.h"

#ifdef BUDDY

// Binary tree levels
#define LEVEL_LIMIT 13

// Maximum number of blocks at level zero
#define BLOCK_LIMIT (1 << (LEVEL_LIMIT - 1))

// Block size can be a power of 2 multiplied by BLOCK_UNIT
#define BLOCK_UNIT 256

#define SIZE(level) ((1 << (level)) * BLOCK_UNIT)

#define IS_POW2(x) (((x) != 0) && (((x) & ((x)-1)) == 0))

typedef struct BlockData {
    bool allocated;
    size_t level;
    struct BlockData *next;
    struct BlockData *prev;
} BlockData;

static BlockData blocks[BLOCK_LIMIT];
static BlockData *bucket[LEVEL_LIMIT];
static size_t levels_count;
static size_t blocks_count;

static void *base_address;

static MemoryInfo memory_info;

size_t log2n(size_t n) {
    size_t ret = 0;
    while (n >>= 1)
        ++ret;
    return ret;
}

int initMemoryManager(void *address, size_t size) {
    // Misaligned address
    if ((size_t)address % sizeof(size_t) != 0)
        return 1;

    // Misaligned size
    if (size % BLOCK_UNIT != 0 || !IS_POW2(size / BLOCK_UNIT))
        return 2;

    size_t level = log2n(size / BLOCK_UNIT);

    // Not enough memory
    if (level == 0)
        return 3;

    // Size off limits
    if (level >= LEVEL_LIMIT)
        return 4;

    // Initialize blocks
    for (size_t i = 0; i < BLOCK_LIMIT; i++) {
        blocks[i].allocated = false;
        blocks[i].level = 0;
        blocks[i].next = NULL;
        blocks[i].prev = NULL;
    }

    // Initialize buckets
    for (size_t i = 0; i < LEVEL_LIMIT; i++)
        bucket[i] = NULL;

    // Initialize main block
    blocks[0].level = level;
    bucket[level] = blocks;

    // Set constants
    levels_count = level + 1;
    blocks_count = 1 << level;
    base_address = address;

    // Initialize memory info
    memory_info.allocated = 0;
    memory_info.free = size;
    memory_info.total = size;


    return 0;
}

// Get memory address of a block
void *getAddress(BlockData *block_ptr) {
    return base_address + (block_ptr - blocks) * BLOCK_UNIT;
}

// Get pointer to buddy
BlockData *getBuddy(BlockData *block_ptr) {
    size_t level_index = (block_ptr - blocks) >> block_ptr->level;
    if (level_index % 2) {
        return block_ptr - (1 << block_ptr->level);
    } else {
        return block_ptr + (1 << block_ptr->level);
    }
}

// Add block to its corresponding bucket
void addBlock(BlockData *block_ptr) {
    block_ptr->next = bucket[block_ptr->level];
    block_ptr->prev = NULL;

    bucket[block_ptr->level] = block_ptr;

    if (block_ptr->next != NULL)
        block_ptr->next->prev = block_ptr;

    block_ptr->allocated = false;
    memory_info.allocated -= SIZE(block_ptr->level);
    memory_info.free += SIZE(block_ptr->level);
}

// Remove block from its current bucket
void removeBlock(BlockData *block_ptr) {
    if (block_ptr->next)
        block_ptr->next->prev = block_ptr->prev;

    if (block_ptr->prev != NULL) {
        block_ptr->prev->next = block_ptr->next;
    } else {
        bucket[block_ptr->level] = block_ptr->next;
    }

    block_ptr->next = NULL;
    block_ptr->prev = NULL;
    block_ptr->allocated = true;
    memory_info.allocated += SIZE(block_ptr->level);
    memory_info.free -= SIZE(block_ptr->level);
}

// If possible, split block into two buddies
int splitBlock(size_t level) {
    // Minimum level reached or no blocks to split
    if (level == 0 || bucket[level] == NULL)
        return -1;

    BlockData *block_ptr = bucket[level];
    removeBlock(block_ptr);
    block_ptr->level = level - 1;

    BlockData *buddy_ptr = getBuddy(block_ptr);
    buddy_ptr->level = level - 1;

    addBlock(buddy_ptr);
    addBlock(block_ptr);

    return 0;
}

// Recursively merge free buddies
void mergeBuddy(BlockData *block_ptr) {
    // Maximum level reached
    if (block_ptr->level == levels_count - 1)
        return;

    BlockData *buddy_ptr = getBuddy(block_ptr);

    if (block_ptr->level != buddy_ptr->level)
        return;

    if (block_ptr->allocated || buddy_ptr->allocated)
        return;

    removeBlock(block_ptr);
    removeBlock(buddy_ptr);

    if (block_ptr > buddy_ptr) {
        BlockData *aux = block_ptr;
        block_ptr = buddy_ptr;
        buddy_ptr = aux;
    }

    block_ptr->level++;
    addBlock(block_ptr);
    mergeBuddy(block_ptr);
}

void *malloc(size_t size) {
    // Get minimum level
    size_t level = log2n(size / BLOCK_UNIT);
    if ((1 << level) * BLOCK_UNIT < size)
        level++;

    // If there is no block of the right size, search for a bigger one and split
    // it the necessary number of times
    if (bucket[level] == NULL) {
        size_t min_above = level + 1;
        while (min_above < levels_count && bucket[min_above] == NULL)
            min_above++;

        // No block found
        if (min_above == levels_count)
            return NULL;

        while (min_above > level)
            splitBlock(min_above--);
    }

    // Take block and return its memory address
    BlockData *block_ptr = bucket[level];
    removeBlock(block_ptr);
    return getAddress(block_ptr);
}

void free(void *ptr) {
    size_t offset = ptr - base_address;
    if (offset % BLOCK_UNIT)
        return;

    BlockData *block_ptr = blocks + (offset / BLOCK_UNIT);
    if (block_ptr >= blocks + blocks_count || !block_ptr->allocated)
        return;

    addBlock(block_ptr);
    mergeBuddy(block_ptr);
}

void getMemoryInfo(MemoryInfo *memory_info_ptr) {
    *memory_info_ptr = memory_info;
}

#endif
