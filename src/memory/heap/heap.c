#include "heap.h"
#include "kernel.h"
#include "status.h"
#include "memory/memory.h"
#include <stdbool.h>


// Validate the heap table
static int heap_validate_table(void* ptr, void* end, struct heap_table* table)
{
    int res = 0;                                                                        // Validate that the pointer provided and the end address is correct for the table given    

    size_t table_size = (size_t)(end - ptr);                                            // Table size
    size_t total_blocks = table_size / NARCHOS_HEAP_BLOCK_SIZE;                         // Total blocks in our heap 
    if (table->total != total_blocks)
    {
        res = -EINVARG;                                                                 // If table total does not equal total blocks then we have a miscalculation so we return invalid argument
        goto out;
    }

out:   
    return res;                                                                                       

}

static bool heap_validate_alignment(void* ptr)
{
    return ((unsigned int)ptr % NARCHOS_HEAP_BLOCK_SIZE) == 0;                         // Return true if there is an alignment
}

// A function for creating a heap
int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)     // Accept heap, pointer to heap data pool, pointer to the end of our heap and heap table
{
    /* Idea is that if we want to create a heap, 
    we call heap_create, pass in the uninitialized heap pointer 
    initialize it, pass in where we want the heap to start and end at 
    and provide a valide heap table one that we filled in. 
    Then we use the 'end' variable to determine if the table is valid or not.
    0 represents no error, negative value represents the error code */
    
    int res = 0;
    res = -EIO;

    if (!heap_validate_alignment(ptr) || heap_validate_alignment(end))                 // It has to be aligned for us to work with the address
    {
        res = -EINVARG;                                                                // Validating alignment of heap, return EINVARG in case alignment is bad
        goto out;
    }

    memset(heap, 0, sizeof(struct heap));                                              // Initialize the whole heap to 0
    heap->saddr = ptr;                                                                 // Set our heap start address to the pointer provided
    heap->table = table;                                                               // Set the heap table in our heap object

    res = heap_validate_table(ptr, end, table);                                        // Validating the table itself
    if (res < 0)
    {
        goto out;
    }

    // Initializing all blocks in the heap entry table to 0
    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;                 // Mark every uninitialized byte in the entry table to 0
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);                   

out:
    return res;
} 

static uint32_t heap_align_value_to_upper(uint32_t val)
{
    if ((val % NARCHOS_HEAP_BLOCK_SIZE) == 0)                                           // Check for alignment                                                                         
    {
        return val;
    }

    val = (val - (val % NARCHOS_HEAP_BLOCK_SIZE));
    val += NARCHOS_HEAP_BLOCK_SIZE;
    return val;
}

// malloc function that takes number of blocks to allocate rather than size
void* heap_malloc_blocks(struct heap* heap, uint32_t total_blocks)
{
    void* address = 0;                                                                 // Find the blocks that we need and return the pointer in data pool

    int start_block = heap_get_start_block(heap, total_blocks);
    if (start_block < 0)
    {
        goto out;
    }

    address = heap_block_to_address(heap, start_block);                                // Converting block to an address

    // Mark the blocks as taken so that the next malloc doesn't return the same address allowing our data to overwrite our data
    heap_mark_blocks_taken(heap, start_block, total_blocks);
out:
    return address;
}

// Creating the heap malloc function
void* heap_malloc(struct heap* heap, size_t size)
{
    size_t aligned_size = heap_align_value_to_upper(size);                             // Gives us the new size
    uint32_t total_blocks = aligned_size / NARCHOS_HEAP_BLOCK_SIZE;                    // Calculate how many blocks to allocate in our entry table to represent the size the caller is asking
    return heap_malloc_blocks(heap, total_blocks);
}

// Creating memory freeing function
void* heap_free(struct heap* heap, void* ptr)
{
    return 0;
}
