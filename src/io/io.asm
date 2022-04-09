section .asm

; Exporting labels
global insb
global insw
global outb 
global outw

; Info learned from: https://c9x.me/x86/html/file_module_x86_id_139.html
insb:                                      ; For one byte
    push ebp                               ; Creating a stack frame
    mov ebp, esp

    xor eax, eax                           ; Making sure eax is 0
    mov edx, [ebp+8]                       ; Moving port into the dx register
    in al, dx 

    pop ebp
    ret

insw:                                      ; For two bytes
    push ebp
    mov ebp, esp

    xor eax, eax
    mov edx, [ebp+8]
    in ax, dx   

    pop ebp
    ret

outb:                                      ; Implementing outb function held in io.h file
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, al                             ; Outputs one byte to the port

    pop ebp
    ret

outw:                                      ; Implementing outw function held in io.h file
    push ebp
    mov ebp, esp

    mov eax, [ebp+12]
    mov edx, [ebp+8]
    out dx, ax                             ; Outputs one byte to the port

    pop ebp
    ret








