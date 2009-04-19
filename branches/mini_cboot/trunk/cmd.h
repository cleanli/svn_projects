#ifndef _CMD_H
#define _CMD_H

#include "interface.h"

#define COM_MAX_LEN 32
#define ENTER_CHAR 0x0d
#define NULL 0

struct command{
     unsigned char * cmd_name;
     void (*cmd_fun)(unsigned char *);
};

void go(unsigned char *p);
void print_mem(unsigned char *p);
void get_file_by_serial(unsigned char *para);
void print_help(unsigned char *para);
void read_mem(unsigned char *para);
void write_mem(unsigned char *para);
void run_clean_boot();

#endif
