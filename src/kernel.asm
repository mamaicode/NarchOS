[BITS 32]                               ; All code after this is seen as 32bits

global _start                           ; Exports the symbol otherwise it wont be publically known
extern kernel_main

CODE_SEG equ 0x08                       ; Kernel code segment
DATA_SEG equ 0x10                       ; Data segment

_start:
    mov ax, DATA_SEG                    ; Setting up data registers
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000                 ; Setting base pointer to point to 0x00200000
    mov esp, ebp                        ; Setting stack pointer to base pointer as well

    ; Enable the A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Remap the master PIC
    mov al, 00010001b                   ; Puts the PIC in initialization mode
    out 0x20, al                        ; Tell master PIC
    
    mov al, 0x20                        ; Interrupt 0x20 is where master ISR should begin
    out 0x21, al 

    mov al, 00000001b                   ; Putting the PIC in x86 mode 
    out 0x21, al                        
                                        ; End remap of the master PIC

    sti                                 ; Enable interrupts

    call kernel_main

    jmp $

times 512-($ - $$) db 0                 ; Now we will start mixing C code into assembly
                                        ; And we need to work around allignment issues
                                        ; 512 % 16 = 0  