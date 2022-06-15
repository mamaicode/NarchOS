#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0x08           // From kernel.asm
#define KERNEL_DATA_SELECTOR 0x10           // From kernal.asm


#define NARCHOS_TOTAL_INTERRUPTS 512        // 512 = 0x200 in Hexadecimal  

#define NARCHOS_HEAP_SIZE_BYTES 104857600   // 100MB, not taking in consideration RAM, so for now it is static
#define NARCHOS_HEAP_BLOCK_SIZE 4096
#define NARCHOS_HEAP_ADDRESS 0x01000000     // Our heap address / More extended memory
#define NARCHOS_HEAP_TABLE_ADDRESS 0x00007E00

#endif