#ifndef _CMD_H
#define _CMD_H

#include "interface.h"

#define COM_MAX_LEN 32
#define ENTER_CHAR 0x0d
#define NULL 0

struct command{
     unsigned char * cmd_name;
     void (*cmd_fun)(unsigned char *);
     unsigned char * cmd_des;
};

void ndbb(unsigned char *p);
void nandr(unsigned char *p);
void nandspr(unsigned char *p);
void ndchkbb(unsigned char *p);
void nander(unsigned char *p);
void nandcp(unsigned char *p);
void go(unsigned char *p);
void pm(unsigned char *p);
void get_file_by_serial(unsigned char *para);
void put_file_by_serial(unsigned char *para);
void print_help(unsigned char *para);
void read_mem(unsigned char *para);
void write_mem(unsigned char *para);
void run_clean_boot();

#endif
