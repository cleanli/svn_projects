#include "global.h"
#include "hardintface.h"
#include <AT89X51.H>

//serial
void setSentFlag()
{
	TI=1;
}

void clrSentFlag()
{
	TI=0;
}
void prepSerialRecv()
{
	RI=0;
}
bit sent()
{
	return TI;
}
bit received()
{
	return RI;
}

void send(uch a)
{
	SBUF=a;
}
uch getRecv()
{
	return SBUF;
}

//485
void set485out()
{
	outen485=1;
}
void set485in()
{
	outen485=0;
}

//led
void ledOn()
{
	led=ON;
}
void ledOff()
{
	led=OFF;
}
void ledRvs()
{
	led=!led;
}

//timer
void resetCounter()
{
	delayct=0;
}
uch getCounter()
{
	return delayct;
}

void timer0(void) interrupt 1 //时间中断函数
{
	t250usct++;
	delayct++;
	commuCt++;
	if(t250usct>=40)
	{
		t250usct=0;		
		t10msct++;
		if(t10msct>=100)
		{
			t10msct=0;
			//1 second 	
		}
		//10ms
	}
	if(commuCt>=10)
	{
		commuCt=0;
		if(bFFirst)
		{
			if(uchDataIndex==0)
			{
				outPort=0xff;
			}
			else
			{
				outPort=instruData.nuchByteData[uchDataIndex-1];
			}
			bFFirst=0;
		}
		else
		{
			if(uchDataIndex==0)
			{
				outPort=0x00;
				uchDataIndex++;
			}
			else
			{
				if(++uchDataIndex>19)uchDataIndex=0;
			}
			bFFirst=1;
		}
	}
}
