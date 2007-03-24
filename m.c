#include <AT89x51.h>
#include <stdlib.h>
#include "gernel.h"
#include "initial.h"
#include "gnlserial.h"
//#include "gnlstring.h"

#define INIT_SP 0x50
#define TIMER 0xe0
xdata unsigned char total_task;
xdata struct task task_list[4];
xdata unsigned char cur_task_index;
xdata unsigned char is_timer0_int;
xdata unsigned int task1ct=0,task2ct=0;

xdata int count=1000;

 


void delay()
{
    unsigned char i;
	for(i=0;i<255;i++);
}

unsigned char get_count(int mount)
{
    int mycount;
    if(count>mount){
	    delay();
		mycount=count;
		delay();
	    mycount-=mount;
		count=mycount;
		return 1;
    }
	return 0;
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
    int get=800;
	P1_1=0;
	task1ct=0;
	while(1){
	  if(get_count(get)){
		P1_1=1;
		task1ct=1;
      }
sendString("ssssss,from task 111111\r\n");
       //switch_task();
    }
}

void task2(void)
{
    int j;
/*
	while(1){
	    if(++task2ct>task1ct)
	        P1_1=!P1_1;
	}
	*/
	int get=900;
	P1_1=0;
	task2ct=0;
	while(1){
	  if(get_count(get)){
		P1_1=1;
		task2ct=1;
      }
	  P1_2=!P1_2;
        //for (j=0;j<254;j++)	delay();
	    sendString("ssssss,from task 22222\r\n");
       //switch_task();
	}
}

void add_task(void(*fun)(void))
{
    //void * f;
	task_list[total_task].task_fun=fun;
	task_list[total_task].regs_bak.sp=INIT_SP;
	*(void**)(&(task_list[total_task].chip_ram[0x7f-INIT_SP-sizeof(fun)]))=fun;//make the value of sp pointing be addr of fun so when chip_ram restored PC will be fun
	//f=&(task_list[total_task].chip_ram[INIT_SP]);
	//	f=fun;
	total_task++;
	//sendString("Adding task");
	//sendInt(total_task);
    //sendString("\r\n");
}



void main()
{
    unsigned char j;
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

	//SP=INIT_SP;
	total_task=1;

	cur_task_index=0;
	add_task(task1);
	add_task(task2);

	EA=1;
	TR0=1;//start the time0 interrupt
	sendString("Hello\r\n");
    while(1)
    {
        //for (j=0;j<254;j++)	delay();
	    sendString("Hello, this is from task 0\r\n");
        //switch_task();
        //P1_2=!P1_2;
    }
	return;
}


