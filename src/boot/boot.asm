ORG 0x7c00              ; Origin
BITS 16                 ; 16bit code only
CODE_SEG equ gdt_code - gdt_start ; Gives us the 0x8 and 0x10 offsets
DATA_SEG equ gdt_data - gdt_start

_start:                 ; New label that jumps to our start label
    jmp short start
    nop                 ; No operation block required by our BIOS https://wiki.osdev.org/FAT

times 33 db 0           ; Creates 33 bytes after short jump, 33 because adding up BIOS parameter block bytes
                        ; So the line 9 is our BIOS parameter block, 
                        ; So if our BIOS starts filling in the values
                        ; It doesnt corrupt our code it just fills in our nobytes
start:
    jmp 0:step2         ; Makes our code segment will change to 0 when it does the jump,
                        ; and our offset 0xc007 will work fine

step2:
    cli ; Clear interrupts
    mov ax, 0x00        ; Data segment
    mov ds, ax          ; And 
    mov es, ax          ; Extra segment. 

    ; By changing the segment register we are taking control,
    ; Rather hoping BIOS sets it up for us

    mov ss, ax
    mov sp, 0x7c00      ; Setting stack pointer 
    sti                 ; Enable interrupt

    

.load_protected:
    cli                 ; Clear interrupts
    lgdt[gdt_descriptor] ; Load global descriptor table, it will go down to our gdt_descriptor it will find size and offset and load our gdt_code and gdt_data
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax        ; Reseting the register now that we have set a bit
    jmp CODE_SEG:load32 ; Switches to code selector and jumps load32 absolute address and then we jump forever
    
;GDT
gdt_start: ; Label to reference it later
gdt_null: ; Null descriptor
    dd 0x0
    dd 0x0

; Offset 0x8, offset in the table  

gdt_code:               ; CS should point to this   WE ARE USING DEFAULTS, WE ARE DOING THIS JUST SO WE CAN ACCESS MEMORY
    dw 0xffff           ; Segment limit first 0-15bits                
    dw 0                ; Base first 0-15 bits
    db 0                ; Base 166-23 bits
    db 0x9a             ; Access byte    CHECK:https://wiki.osdev.org/Global_Descriptor_Table
    db 11001111b        ; High 4 bit flags and the low 4 bit flags
    db 0                ; Base 24-31 bits


; Offset 0x10, for our data segment and stack segment and all that type of stuff

gdt_data:               ; Should be linked to DS,SS,ES,FS,GS
    dw 0xffff           ; Segment limit first 0-15bits                
    dw 0                ; Base first 0-15 bits
    db 0                ; Base 166-23 bits
    db 0x92             ; Access byte    CHECK:https://wiki.osdev.org/Global_Descriptor_Table
    db 11001111b        ; High 4 bit flags and the low 4 bit flags
    db 0                ; Base 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start-1 ; Gives us the size of descriptor
    dd gdt_start        ; This is our offset

[BITS 32]               ; Just to mention when we are in 32 bit protected mode we can no longer use BIOS!
load32:
    mov eax, 1          ; Represents the startng sector, 1
    mov ecx, 100        ; Total number of sectors we want to load
    mov edi, 0x0100000  ; edi contanins address we want to load sectors into
    call ata_lba_read   ; Label that talks with drive and loads sectors into memory
    jmp CODE_SEG:0x0100000


ata_lba_read:          ; Writing the driver to get the kernel loaded
    mov ebx, eax,      ; Backup the LBA
                       ;Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24        ; eax will be shifted 24 bits so it will contain 8 highest bits now 32-24
    or eax, 0xE0       ; Selects the master drive (since we have slave and master)
    mov dx, 0x1F6      ; Port it expects to write 8 bits into
    out dx, al 
    ; Finished sending the highest 8 bits of the LBA

; Sending total sectors to read to the hard disk controller
mov eax,ecx
mov dx, 0x1F2
out dx, al
; Finished sending the total sectors to read

; Send more bits of the LBA
mov eax, ebx         ; Restoring the backup LBA
mov dx, 0x1F3
out dx, al
; Finished sending more bits of the LBA

; Send more bits of the LBA
mov dx, 0x1F4
mov eax, ebx;
shr eax, 8
out dx, al
; Finished sending moer bits of the LBA

; Send upper 16 bits of LBA
mov dx, 0x1F5
mov eax, ebx        ; Restore the backup LBA
shr eax, 16         ; Shift by 16bits
out dx, al          ; Output to controller
; Finished sending upper 16 bits of LBA


mov dx, 0x1f7
mov al, 0x20
out dx, al

; Read all sectors into memory
.next_sector:
    push ecx       ; Pushing ecx register to stack



; Checking if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx     ; Read port 0x1f7 into the al register
    test al, 8 
    jz .try_again ; Jump back if it doesnt fail


; Need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw          ; Reading the 256 word from the port 0x1F0 and storing it into 0x0100000, 256 words i.e 256 bits
    pop ecx           ; ecx is the total number of sectors we want to read so we pop it to restore that sector number
    loop .next_sector ; Restore ecx to sector count, so this will go to next sector and decrement ecx so
                      ; when it loops around it is one sector less to read 
; End of reading sectors into memory
    ret               ; Return from the subroutine

times 510-($ - $$) db 0 ; fill at least 510 bytes of data 
dw 0xAA55               ; Since intel machine is little endian 
                        ; Bytes get flipped when working with words
                        ; We go for 0xAA55 aka 0x55AA AKA 511/512 byte, because
                        ; It should contain a boot signature 
                        ; If BIOS finds the signature 
                        ; It will load that sector into origin address 
                        ; And will execute interrupt from bios 
                        ; From that address running our bootloader
                        ; Also to mention if we use 510 bytes
                        ; Our command wont output anything 
                        ; But if we don't, it'll pad the rest with 0
                        ; Lets say we use only five bytes in the code
                        ; Then the extra 10 bytes will be filled 
                        ; All the way through with zeros
                        ; And this allows us to just go to next line with dw

