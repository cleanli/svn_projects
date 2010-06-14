#ifndef _PRINT_H
#define _PRINT_H 

#include "type.h"
#include "interface.h"

void print_string(const unsigned char *s);
unsigned char halfbyte2char(unsigned char c);
uint div(uint beichushu, uint chushu);
unsigned char * num2str(uint jt, unsigned char * s, unsigned char n);
void print_uint(uint num);
void print_hex(uint num);
void lprint(const unsigned char * fmt, ...);
#endif
