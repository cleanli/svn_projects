#include <AT89x51.h>
#include <stdlib.h>
#include "gernel.h"
#include "initial.h"
#include "gnlserial.h"
#include "stdarg.h"
#include "gnlstring.h"


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



void delay()
{
    unsigned char i;
	for(i=0;i<255;i++);
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

void task1(void)
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

void task2(void)
{

}

void add_task(void(*fun)(void))
{
    //void * f;
	task_list[total_task].task_fun=fun;
	task_list[total_task].regs_bak.sp=INIT_SP;
	*(void**)(&(task_list[total_task].chip_ram[0x7f-INIT_SP-sizeof(fun)]))=fun;//make the value of sp pointing be addr of fun so when chip_ram restored PC will be fun
	task_list[total_task].status=0;
	//f=&(task_list[total_task].chip_ram[INIT_SP]);
	//	f=fun;
	total_task++;
	//sendString("Adding task");
	//sendInt(total_task);
    //sendString("\r\n");
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

	//SP=INIT_SP;
	total_task=1;

	cur_task_index=0;
	add_task(time_second);
	add_task(task1);
	//add_task(task2);



	EA=1;
	TR0=1;//start the time0 interrupt

	//shell like interface
	printk("Now task start runing ...\r\n");
	com_buf_p=0;
	lmemset(command_buf,0,MAX_COM_LEN);
	printk("CleanOS@51>");
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
    printk("\r\n");
	if(!command_buf[0])return;
	if(!lstrcmp(command_buf,"time"))printk("The count is %d%d\r\n",time_sec);
    else printk("Unknow command:%s\r\n",command_buf);
}

