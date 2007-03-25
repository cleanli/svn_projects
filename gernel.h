#ifndef _GERNEL_H
#define _GERNEL_H

#define INIT_SP 0x50
#define TIMER 0xe0
#define VERSION "0.3"
#define MAX_TASK 4
#define SECOND_COUNTS 223
#define SECOND_LED P1_2

struct regs{
    unsigned char psw;//0x80
	unsigned char dpl;//0x81
	unsigned char dph;
    unsigned char b;//0x83
    unsigned char acc;
    unsigned char sp;//0x85
};
    
//typedef struct task * task_p;
struct task{
     unsigned char chip_ram[0x80];//0-0x7f
	 struct regs regs_bak;//0x80-0x85
	 void (*task_fun)(void);//0x86-0x88
     unsigned char status;//0x89
};

#endif
