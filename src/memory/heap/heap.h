#ifndef HEAP_H
#define HEAP_H
#include <stdint.h> 
#include "config.h"
#include <stddef.h>

// First four bits that represent the type and show if the entry is free or taken
#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0x00

// Implementing bitmask for two topmost bits, which determine if we have more blocks next to us or if it's the first block
#define HEAP_BLOCK_HAS_NEXT 0b10000000
#define HEAP_BLOCK_IS_FERE  0b01000000

// Making a type to the bitmask
typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;           // 8 bits in the entry table, one entry represents 8 bits

// Creating the heap entry table
struct heap_table
{
    // Creating a pointer here to a place where our entries will reside
    HEAP_BLOCK_TABLE_ENTRY* entries;
    size_t total;                                       // Helps to understand how many entries we have, 100MB stored in the 'total' variable
};

// Implementing a structure for our heap
struct heap
{
    struct heap_table* table;                           // Pointer to our heap table 
    void* saddr;                                        // Start address of the heap data pool
};

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table);

#endif
