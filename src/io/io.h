#ifndef IO_H
#define IO_H

// Check file io.asm for assembly instructions for these function prototypes

unsigned char insb(unsigned short port);                // Reads one byte from the given port
unsigned short insw(unsigned short port);               // Reads word(2 bytes) from the given port

void outb(unsigned short port, unsigned char val);      // Value we'd like to output to the port (1 byte)
void outw(unsigned short port, unsigned short val);     // Outputting 2 bytes to the port

#endif