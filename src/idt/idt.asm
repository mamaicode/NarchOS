section .asm                    ; Telling processor where the interrupt descriptor table is
                                ; We do this by LIDT processor instruction
extern int21h_handler        
extern no_interrupt_handler

global int21h                   ; Exporting the function
global no_interrupt                         ; Exporting the function
global idt_load                 ; So it exports the symbol

idt_load:                       ; Writing assembly that's compatible with C
    push ebp                    ; Pushing the base pointer
    mov ebp, esp                ; Moving stack pointer into the base pointer so that we have a reference to our frame             

    mov ebx, [ebp+8]            ; Moves the value from ebp+8 into ebx, and the value is the pointer provided to us
    lidt [ebx]                  ; Implementing LIDT processor instruction
    pop ebp                     ; Popping the base pointer
    ret                         ; Return


int21h:                         ; Keyboard interrupt
    cli                         ; Clear interrupts
    pushad                      ; https://faydoc.tripod.com/cpu/pushad.htm, pushes registers
    call int21h_handler         ; Function
    popad                       ; Restores all registers
    sti                         ; Start interrupts again
    iret                        ; Return

no_interrupt:                   ; Assembly function for when there are no interrupts
    cli                         ; Clear interrupts
    pushad                      ; Push registers
    call no_interrupt_handler   ; Function
    popad                       ; Restore all registers
    sti                         ; Start interrupts again
    iret                        ; Return





