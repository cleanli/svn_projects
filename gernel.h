#ifndef _GERNEL_H
#define _GERNEL_H

#include <at89x51.h>
#include <stdlib.h>

#define INIT_SP 0x5d
#define TIMER 0xe0
#define VERSION "0.3"
#define MAX_TASK 5
#define SECOND_COUNTS 223
#define SECOND_LED P1_2
#define MAX_COM_LEN 0x80
#define TOTAL_CMDS (sizeof(cmd_list)/sizeof(struct command))
#define PUSHS_IN_INT 4


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
	 void (code*task_fun)(void);//0x86-0x87
     unsigned char status;//0x88
	 unsigned char *task_name;//89-8B
     struct task xdata * next_task;//8C 8D
	 unsigned char pid;
     struct task xdata * prev_task;//8C 8D
};

struct command{
     unsigned char * cmd_name;
     void (*cmd_fun)(void xdata*);
};

extern pdata unsigned char cur_pid;
//xdata struct task task_list[MAX_TASK];
extern pdata struct task xdata* task_head;
extern pdata struct task xdata* cur_task;
extern pdata unsigned char soft_int;
extern pdata unsigned int task1ct,task2ct;
extern pdata unsigned char intrpt_count;
extern pdata unsigned int time_sec;
extern pdata unsigned char xdata*command_buf;
extern pdata unsigned char com_buf_p;
extern pdata unsigned char dont_switch_task;


extern pdata int count;

#endif
