#include "config.h"
#include "kheap.h"
#include "heap.h"
#include "kernel.h"

// Declaring structures
struct heap kernel_heap;                    // Kernel heap
struct heap_table kernel_heap_table;        // Heap table

// 100MB heap -> 1024^2 * 100 / 4096(blocksize) = 25600 kernel heap table entries
void kheap_init()
{
    int total_table_entries = NARCHOS_HEAP_SIZE_BYTES / NARCHOS_HEAP_BLOCK_SIZE;
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(NARCHOS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;

    // Calculating our env variable
    void* end = (void*)(NARCHOS_HEAP_ADDRESS + NARCHOS_HEAP_SIZE_BYTES);          // Points us to the end of our table
    int res = heap_create(&kernel_heap, (void*)(NARCHOS_HEAP_ADDRESS), end, &kernel_heap_table);
    if (res < 0)
    {
        print("Failed to create heap\n");
    }                                  
}
