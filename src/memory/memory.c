#include "memory.h"

// Creating our own memset function
void* memset(void* ptr, int c, size_t size)
{

    char* c_ptr = (char*) ptr;
    for (int i = 0; i < size; i++)
    {
        c_ptr[i] = (char) c;
    }
    return ptr;                       // Returns pointer, said so on the C specification provided
}

