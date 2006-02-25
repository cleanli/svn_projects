#include "initial.h"
#include "global.h"
#include "hardintface.h"
#include "lstring.h"

void totalinitial()
{
	IE=0;//disable all interrupt
	set485in();
	cpuinitial();
	serialinitial();
	timerinitial();
	variableinitial();
	EA=1;
}
void variableinitial()
{
	t250usct=0;
	delayct=0;
	t10msct=0;
	delayct=0;
	commuCt=0;
	uchDataIndex=0;
	lmemset(instruData.nuchByteData,0,20); 
	bFFirst=1;
}
void serialinitial()
{
	PCON|=0x80;//baute rate double
	TH1=0xfd;
	TL1=0xfd;//19200,11.0592
	SCON=0x50;//8,1 stop,receive enable
	setSentFlag();//indicate send over
	prepSerialRecv();
	TR1=1;//start timer 1
}
void timerinitial()
{
	TH0=26;
	TL0=0;//250us,11.0592
	TR0=1;
	ET0=1;//timer0 interrupt enable
}

void cpuinitial()
{
	PSW=0;//bank 0
	IP=0x02;//timer0 interrupt priority high
	TMOD=0x22;//TL counter, TH save the value 
	TR0=0;
	TR1=0;
}
