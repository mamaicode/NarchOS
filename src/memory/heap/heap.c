#include "heap.h"

// A function for creating a heap
int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table)      // Accept heap, pointer to heap data pool, pointer to the end of our heap and heap table
{
    /* Idea is that if we want to create a heap, 
    we call heap_create, pass in the uninitialized heap pointer 
    initialize it, pass in where we want the heap to start and end at 
    and provide a valide heap table one that we filled in. 
    Then we use the 'end' variable to determine if the table is valid or not.*/

    return 0;                                    // Returns an integer, 0 represents no error, negative value represents the error code

} 
