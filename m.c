#include <AT89x51.h>
#include <stdlib.h>
#include "gernel.h"
#include "initial.h"
#include "gnlserial.h"
#include "stdarg.h"
#include "gnlstring.h"
#include "atomic.h"
#include "mm.h"
#include "gnlasc.h"


pdata unsigned char cur_pid;
//xdata struct task task_list[MAX_TASK];
pdata struct task xdata* task_head=NULL;
pdata struct task xdata* cur_task=NULL;
pdata unsigned char soft_int;
pdata unsigned int task1ct=0,task2ct=0;
pdata unsigned char intrpt_count;
pdata unsigned int time_sec;
pdata unsigned char xdata*command_buf;
pdata unsigned char com_buf_p;
pdata unsigned char dont_switch_task;


//pdata int count=1000;

void handle_cmd(); 
void print_time(void xdata*);
void print_help(void xdata*);
void print_task(void xdata*);
void print_free(void xdata*);
void kill_task(void xdata*);

code const struct command cmd_list[]=
{
    {"time",print_time},
	{"help",print_help},
	{"ps",print_task},
	{"free",print_free},
	{"kill",kill_task},
};



int printk(const char code* fmt, ...)
{
    const char *s;
    int d;
    va_list ap;

    va_start(ap, fmt);
    while (*fmt) {
        if (*fmt != '%') {
			sendSerialByte(*fmt++);
            continue;
        }
        switch (*++fmt) {
		    case '%':
			    sendSerialByte(*fmt);
				break;
            case 's':
                s = va_arg(ap, const char *);
                sendString(s);
                break;
            case 'd':
                d = va_arg(ap, int);
                sendInt(d);
                break;
			case 'c':
                d = va_arg(ap, char);
                sendInt(d);
                break;
			case 'x':
                d = va_arg(ap, char);
                sendASCByte(d);
                break;
            /* Add other specifiers here... */             
            default: 
                sendSerialByte(*(fmt-1));
				sendSerialByte(*fmt);
                break;
        }
        fmt++;
    }
    va_end(ap);
    return 1;   /* Dummy return value */
}

void delay(unsigned sec)
{
	int tmp;
	tmp=time_sec;
	while(time_sec<tmp+sec);
}


void time_second()
{
    while(1){
	    if(intrpt_count>SECOND_COUNTS){
		    intrpt_count=0;
			time_sec++;
			SECOND_LED=!SECOND_LED;
			//printk("%d\r\n",*((int*)((&time_sec)+4)));
			//printk("%d%d\r\n",time_sec);
			//printk("%x%x\r\n",time_sec);
			//sendInt(*((int*)(&time_sec)+1));
			//sendInt(7);
			//printk("dd\n\r");
		}
	}
}

void p11m(void)
{
//    int i;
/*
	while(1){
	    if(++task1ct>task2ct)
	        P1_0=!P1_0;
	}
*/
	P1_1=1;
	while(1){
	  if(!P1_1){
		printk("P1_1 down!\r\n");
      }
//sendString("ssssss,from task 111111\r\n");
       //switch_task();
    }
}
/*
pdata atomic test_lock;
pdata struct quene xdata* test_sleep=NULL;
void open()
{
     while(atomic_test_inc(&test_lock)){
         printk("%s pending\r\n",cur_task->task_name);
	     task_sleep(&test_sleep);
	 }
	 printk("%s open\r\n",cur_task->task_name);
}

void close()
{
     printk("%s close\r\n",cur_task->task_name);
	 atomic_dec(&test_lock);
     task_wake(&test_sleep);
}


void task1(void)
{   
	while(1){
      open();
	  delay(10);
	  close();
	  delay(3);
    }
}
void task2(void)
{
	while(1){
      open();
	  delay(6);
	  close();
	  delay(5);
    }
}
*/
void xdata* kmalloc(unsigned int bytes);

int add_task(void(code*fun)(void),unsigned char * name)
{
    //void * f;
    struct task xdata*new = (struct task xdata*)kmalloc(sizeof(struct task));

    if(new == 0)return -1;
	new->task_fun=fun;
	new->regs_bak.sp=INIT_SP+PUSHS_IN_INT;
	//*(void**)(&(new->chip_ram[0x7f-INIT_SP-sizeof(fun)]))=fun;//make the value of sp pointing be addr of fun so when chip_ram restored PC will be fun
	*(void**)(&(new->chip_ram[INIT_SP-1]))=fun;
	*(void**)(&(new->chip_ram[INIT_SP-3]))=fun;
	new->status=0;
	new->task_name=name;
	new->pid=cur_pid++;
    new->next_task=task_head;
	new->prev_task=NULL;
	if(task_head != NULL)task_head->prev_task=new;
    task_head=new;
	//f=&(task_list[total_task].chip_ram[INIT_SP]);
	//	f=fun;
	//total_task++;
	//sendString("Adding task");
	//sendInt(total_task);
    //sendString("\r\n");
//	nexttask:
//	cur_task=cur_task->next_task;
//	if(cur_task == NULL)cur_task=task_head;
//	if(!cur_task -> status)goto nexttask;
}

unsigned char rm_task(uch pid)
{
    struct task xdata*p;

	p=task_head;
	while(p != NULL){
	    if( p->pid == pid){
		    if(cur_task == p)return 0;
			if(p->prev_task != NULL)
			    p->prev_task->next_task=p->next_task;
			else
			    task_head=p->next_task;
			if(p->next_task != NULL)p->next_task->prev_task=p->prev_task;
			kfree(p);
			return 1;
		}
		p=p->next_task;
	}
	return 0;
}



void main()
{
    unsigned char tmp;

 /*
    while(1)
    {
        for (j=0;j<254;j++)delay();
        P1_2=!P1_2;
    }
	*/

    //PSW=0x08;
	EA=0;
	TMOD=0x21;
	TH0=TIMER;
	TL0=0;
	ET0=1;
	soft_int=0;

	//serialinitial
	PCON|=0x80;//baute rate double				PCON(SMOD--- --- --- GF1 GF0 PD  IDL)(0XXX0000)
	SCON=0x50;//8,1 stop,receive enable			SCON(SM0 SM1 SM2 REN TB8 RB8 TI  RI )
	TH1=0xff;
	TL1=TH1;//19200,11.0592
	TR1=1;//start timer 1
	TI=1;
    prepSerialRecv();


	printk("\r\n\r\nHello, this is CleanOS@51 V%s\r\n",VERSION);
	cur_pid=0;
	SP=INIT_SP;
	//total_task=1;
	command_buf=(unsigned char*)kmalloc(MAX_COM_LEN);
    dont_switch_task=0;

    add_task(main,"Shell");
	cur_task=task_head;

	add_task(time_second,"Timer counter");
	add_task(p11m,"P11 monitor");
	//add_task(task1,"1");
	//add_task(task2,"2");
	//add_task(task2);
//	printk("now free meme is %d Bts\r\n",get_free());
//		printk("%d bytes free\r\n",get_free());





	//shell like interface
	printk("Now task start runing ...\r\n");
	com_buf_p=0;
	//lmemset(command_buf,0,MAX_COM_LEN);
	printk("CleanOS@51>");
	//lstrcmp("time","time");
	EA=1;
	TR0=1;//start the time0 interrupt
    while(1)
    {
		while(!received());
		prepSerialRecv();
		tmp=SBUF;
		if(tmp != 0x0d){
		    if(com_buf_p<MAX_COM_LEN-1){
			    command_buf[com_buf_p++]=tmp;
                sendSerialByte(tmp);
			}
		}
		else{
            handle_cmd();
			lmemset(command_buf,0,MAX_COM_LEN);
            printk("CleanOS@51>");
			com_buf_p=0;
		}    
		//printk("Received Byte 0x%x\r\n",tmp);
    }
	return;
}

void handle_cmd()
{
    unsigned char i, code*p_cmd,xdata*p_buf;
    printk("\r\n");
	p_buf=command_buf;
	if(!command_buf[0])return;
    for(i=0;i<TOTAL_CMDS;i++){
	    p_cmd=cmd_list[i].cmd_name;
		while(*p_buf++ == *p_cmd)p_cmd++;
		p_buf--;
		if(!(*p_cmd) || !(*p_buf)){
            cmd_list[i].cmd_fun(p_buf);
            return;
        }
    }
//if(!lstrcmp(command_buf,"time"))printk("The count is ");
    printk("Unknow command:%s\r\n",command_buf);
}

void kill_task(void xdata*p)
{
    if(rm_task(str_to_byte((uch xdata*)p)))
	    printk("Task %s killed!\r\n",p);
	else
	    printk("Task %s failed to kill!\r\n",p);
}

void print_time(void xdata*p)
{
    printk("Now count is %d\r\n",time_sec);
}

void print_help(void xdata*p)
{
    unsigned char i;
    for(i=0;i<TOTAL_CMDS;i++){
	    printk("%s\r\n",cmd_list[i].cmd_name);
    }
}

void print_task(void xdata*p)
{
    struct task xdata* t_p;
	char code*code* task_status[]={" R"," P"};

	printk ("    PID\tStatus\tName\r\n");
    for(t_p=task_head;t_p != NULL;t_p=t_p->next_task){
	    printk("%c\t%s\t%s\r\n",t_p->pid,task_status[t_p->status],t_p->task_name);
    }
}

void print_free(void xdata*p)
{
    printk("Free memory is %d bytes.\r\n",get_free());
}
/*
void Timer0(void) interrupt 1 using 1
{
    unsigned char data * ci;
	if(soft_int)
	    soft_int=0;
	else{ 
	    intrpt_count++;
		TH0=TIMER;
		TL0=0;
	}
    for(ci=0;ci<0x80;ci++)
        cur_task->chip_ram[(unsigned char)ci]=*ci;
    cur_task->regs_bak.sp=SP;

    do{
	    cur_task=cur_task->next_task;
	    if(cur_task == NULL)cur_task=task_head;
    }while(cur_task->status);

    for(ci=0;ci<0x08;ci++)
        *ci=cur_task->chip_ram[(unsigned char)ci];
    for(ci=0x10;ci<0x80;ci++)
        *ci=cur_task->chip_ram[(unsigned char)ci];
    SP=cur_task->regs_bak.sp;
}
*/
