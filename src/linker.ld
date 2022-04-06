/* Our linker script, when will link our object files this will ensure the origin start at 1mb */

ENTRY(_start)              /* Program entry point */
OUTPUT_FORMAT(binary)      /* We will output as flat binary file we dont need to load headers */
SECTIONS
{
    . = 1M;                /* Our kernel will be loaded into 1mb in memory. sections will start at 1mb in memory */
    .text :
    {
        *(.text)           /* Our text section */
    }

    .rodata : ALIGN(4096)  /* We want to work with aligned memory */
    {
        *(.rodata)         /* Defining our read only data */
    }

    .data : ALIGN(4096) 
    {
        *(.data)           /* Defining data section */
    }

    .bss : ALIGN(4096)
    {
        *(COMMON)          /* Defining our bss section */
        *(.bss) 
    }

    .asm : ALIGN(4096)
    {

        *(.asm)            /* Defining our assembly section */
    }


}

/* We are making sure everything in text section is aligned by 16 bytes, 
we know this because the C compiler does this by default and also we aligned it in kernel.asm file
by forcing it to be 512 bytes long always, we are ensuring alignment with all our C code */