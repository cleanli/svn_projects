#ifndef _GERNEL_H
#define _GERNEL_H

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
