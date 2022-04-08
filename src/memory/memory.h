#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
// Creating a memset function to nail everything in idt.c file
void* memset(void* ptr, int c, size_t size);
#endif