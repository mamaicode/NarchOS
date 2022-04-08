#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"
// Creating structures
struct idt_desc idt_descriptors[NARCHOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;
// Implementing a function prototype, so we know how to talk to it
extern void idt_load(struct idtr_desc* ptr);

void idt_zero()                                                 // Print function for testing void idt_init
{
    print("Divide by zero error\n");
}

// ALL INFO PROVIDED ON https://wiki.osdev.org/Interrupt_Descriptor_Table
/* Creating the IDT set functions,   
which allows us to set interrupts and addresses we want it to go to */
void idt_set(int interrupt_no, void* address)
{
    struct idt_desc* desc = &idt_descriptors[interrupt_no];     // Creating a pointer to the descriptor and passing interrupt number
    desc->offset_1 = (uint32_t) address & 0x0000ffff;           // Setting off one of the descriptors
    desc->selector = KERNEL_CODE_SELECTOR;                      // Setting the selector, it has been set in kernel.asm
    desc->zero = 0x00;                                          // Setting our zero to zero
    desc->type_attr = 0xEE;                                     // The reason its not 0E, is because we have to take in account to set the other bits 
    desc->offset_2 = (uint32_t) address >> 16;                  // Shifted right by 16 because in IDT Offset higher part of the offset is used
}

// Creating a function that sets up the interrupt descriptor table
void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));        // Creates bunch of null descriptors for us
    idtr_descriptor.limit = sizeof(idt_descriptors) -1;         // Setting up the IDTR
    idtr_descriptor.base = (uint32_t) idt_descriptors;

    idt_set(0, idt_zero);                                       // Calling a function as a test

    // Load the interrupt descriptor table
    idt_load(&idtr_descriptor);
}
