#include "kernel.h"
#include <stddef.h>
#include <stdint.h>
#include "idt/idt.h"

/* Creating pointer to an absolute memory 
using unsigned int as each vide_mem is 1 byte */
uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | c;
    /* We shift color to the left by 8 bits, 
    we or it with the char provided to us
    basically returns us the unsigned integer that
    we need and the correct endianess so that when we 
    set the video memory it is set correctly */
}

/* Print routine */
void terminal_putchar(int x, int y, char c, char colour)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

/* terminal_writechar function will just take a character and color
will write it to terminal and then it will increment a position 
like a column or row which will ensure next time we call termina_writechar
it writes to a different column, whereas terminal_putchar takes x and y 
and doesn't keep track of column or rows */
void terminal_writechar(char c, char colour)
{
    if (c == '\n')                  // Implementing a new line to print out a new string 
    {
        terminal_row += 1;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col += 1;              // Ensures the color is incrimented by one 
    if (terminal_col >= VGA_WIDTH)  // If we get back to the row we have to loop back 
    {
        terminal_col = 0;           // If we get to the end of the line we start on new line 
        terminal_row += 1;
    }
}

/* Initialization function
will basically loop through the terminal 
and essentially clear it all for us, 
so that all the BIOS stuff is blank */
void terminal_initialize()
{
    video_mem = (uint16_t*)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;
    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {   // Calculating index 
            terminal_putchar(x, y, ' ', 0);
        }
    }   
}

// Creating size len function to count char in a string in order to print it out 
size_t strlen(const char* str)
{
    size_t len = 0;
    while(str[len])
    {
        len++;
    }

    return len;
}

// Our print routine to print string to terminal 
void print(const char* str)
{
    size_t len = strlen(str);
    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}

void kernel_main()
{
    terminal_initialize();
    print("Hello world!\ntest");

    // Initialize the interrupt descriptor table
    idt_init();
}