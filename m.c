#include <AT89x51.h>
#include <stdlib.h>
#include "gernel.h"
#include "initial.h"
#include "gnlserial.h"
#include "stdarg.h"
#include "gnlstring.h"
#include "atomic.h"


xdata unsigned char total_task;
xdata struct task task_list[MAX_TASK];
xdata unsigned char cur_task_index;
xdata unsigned char is_timer0_int;
xdata unsigned int task1ct=0,task2ct=0;
xdata unsigned char intrpt_count;
xdata unsigned long time_sec;
xdata unsigned char command_buf[MAX_COM_LEN];
xdata unsigned char com_buf_p;


xdata int count=1000;

void handle_cmd(); 
void print_time();
void print_help();
void print_task();

const struct command cmd_list[]=
{
    {"time",print_time},
	{"help",print_help},
	{"ps",print_task}
};



int printk(const char * fmt, ...)
{
    xdata const char *s;
    xdata int d;
    xdata va_list ap;

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



void time_second()
{
    while(1){
	    if(intrpt_count>SECOND_COUNTS){
		    intrpt_count=0;
			time_sec++;
			SECOND_LED=!SECOND_LED;
			//printk("%d\r\n",*((int*)((&time_sec)+4)));
			//printk("%d%d\r\n",time_sec);
			//printk("%x%x%x%x\r\n",time_sec);
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
xdata uch test_sleep[MAX_TASK];
xdata atomic test_lock;

void open()
{
     while(atomic_test_inc(&test_lock)){
         printk("%c pending\r\n",cur_task_index);
	     task_sleep(test_sleep);
	 }
	 printk("%c open\r\n",cur_task_index);
}

void close()
{
     printk("%c close\r\n",cur_task_index);
	 atomic_dec(&test_lock);
     task_wake(test_sleep);
}

void delay(unsigned sec)
{
	long tmp;
	tmp=time_sec;
	while(time_sec<tmp+sec);
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

void add_task(void(*fun)(void),unsigned char * name)
{
    //void * f;
	task_list[total_task].task_fun=fun;
	task_list[total_task].regs_bak.sp=INIT_SP;
	*(void**)(&(task_list[total_task].chip_ram[0x7f-INIT_SP-sizeof(fun)]))=fun;//make the value of sp pointing be addr of fun so when chip_ram restored PC will be fun
	task_list[total_task].status=0;
	task_list[total_task].task_name=name;
	//f=&(task_list[total_task].chip_ram[INIT_SP]);
	//	f=fun;
	total_task++;
	//sendString("Adding task");
	//sendInt(total_task);
    //sendString("\r\n");
}


void * kmalloc(unsigned int bytes);
void main()
{
    unsigned char tmp;
	int * mem_malloc_test;
 /*
    while(1)
    {
        for (j=0;j<254;j++)delay();
        P1_2=!P1_2;
    }
	*/
    mem_malloc_test=(int*)kmalloc(40);

	EA=0;
	TMOD=0x21;
	TH0=TIMER;
	TL0=0;
	ET0=1;
	is_timer0_int=0;

	//serialinitial
	PCON|=0x80;//baute rate double				PCON(SMOD--- --- --- GF1 GF0 PD  IDL)(0XXX0000)
	SCON=0x50;//8,1 stop,receive enable			SCON(SM0 SM1 SM2 REN TB8 RB8 TI  RI )
	TH1=0xff;
	TL1=TH1;//19200,11.0592
	TR1=1;//start timer 1
	TI=1;
    prepSerialRecv();

	printk("\r\n\r\nHello, this is CleanOS@51 V%s\r\n",VERSION);

	SP=INIT_SP;
	total_task=1;
	cur_task_index=0;
    task_list[0].task_name="Shell like Interface";
	add_task(time_second,"Timer counter");
	add_task(p11m,"P1.1 monitor");
	add_task(task1,"1");
	add_task(task2,"2");
	//add_task(task2);



	EA=1;
	TR0=1;//start the time0 interrupt

	//shell like interface
	printk("Now task start runing ...\r\n");
	com_buf_p=0;
	lmemset(command_buf,0,MAX_COM_LEN);
	printk("CleanOS@51>");
	//lstrcmp("time","time");
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
    unsigned char i;
    printk("\r\n");
	if(!command_buf[0])return;
    for(i=0;i<TOTAL_CMDS;i++){
	    if(!lstrcmp(command_buf,cmd_list[i].cmd_name)){
            cmd_list[i].cmd_fun();
            return;
        }
    }
//if(!lstrcmp(command_buf,"time"))printk("The count is ");
    printk("Unknow command:%s\r\n",command_buf);
}

void print_time()
{
    printk("Now count is %d%d\r\n",time_sec);
}

void print_help()
{
    unsigned char i;
    for(i=0;i<TOTAL_CMDS;i++){
	    printk("%s\r\n",cmd_list[i].cmd_name);
    }
}

void print_task()
{
    unsigned char i;
	char ** task_status[]={"Running","Pending"};

	printk ("PID\tStatus\tName\r\n");
    for(i=0;i<total_task;i++){
	    printk("%c\t%s\t%s\r\n",i,task_status[task_list[i].status],task_list[i].task_name);
    }
}

