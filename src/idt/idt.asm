section .asm                    ; Telling processor where the interrupt descriptor table is
                                ; We do this by LIDT processor instruction
global idt_load                 ; So it exports the symbol
idt_load:                       ; Writing assembly that's compatible with C
    push ebp                    ; Pushing the base pointer
    mov ebp, esp                ; Moving stack pointer into the base pointer so that we have a reference to our frame             

    mov ebx, [ebp+8]            ; Moves the value from ebp+8 into ebx, and the value is the pointer provided to us
    lidt [ebx]                  ; Implementing LIDT processor instruction
    pop ebp                     ; Popping the base pointer
    ret                         ; Return
