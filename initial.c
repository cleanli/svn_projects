#include "initial.h"

uch getTmrSet(uint interval)
{
	ULONG tmp;
	tmp=(ULONG)interval*1000;
	tmp=tmp/TIMERCONST;
	tmp=tmp-1;
	tmp=255-tmp;
	return tmp;
}

uch getSrlSet(uint btv)
{
	uch tmp;
	tmp=MAXBOLTRATE/btv-1;
	return 0xff-tmp;
}

void chipInitial()
{
	IE=0;		//disable all interrupt: 		IE  (EA  --- --- ES  ET1 EX1 ET0 EX0)
	TR0=0;
	TR1=0;		//stop the timer				TCON(TF1 TR1 TF0 TR0 IE1 IT1 IE0 IT0)

	//cpuinitial
	PSW=0;		//bank 0:						PSW (CY  AC  F0  RS1 RS0 OV  --- P  )(XX000000)
	IP=0x02;//timer0 interrupt priority high	IP  (--- --- PT2 PS  PT1 PX1 PT0 PT0)

	//timerinitial
	TMOD=0x22;	//TL counte, TH save the value 	TMOD(GATE C/T -M1 M0 GATE C/T- M1 M0)
	TH0=getTmrSet(250);
	TL0=0;//250us,11.0592
	//TR0=1;
	//ET0=1;//timer0 interrupt enable

	//serialinitial
	PCON|=0x80;//baute rate double				PCON(SMOD--- --- --- GF1 GF0 PD  IDL)(0XXX0000)
	SCON=0x50;//8,1 stop,receive enable			SCON(SM0 SM1 SM2 REN TB8 RB8 TI  RI )
	TH1=getSrlSet(57600);
	TL1=0;//19200,11.0592
	TR1=1;//start timer 1

	EX0=0;//extern int0 enable(key int)

	//EA=1;
}
