#include "modbus.h"
#include "hardintface.h"
#include "timer.h"
#include "serial.h"
#include "asc.h"
#include "check.h"
#include "lstring.h"

uch sendModbus(uch*m,uch n)
{
	uch a;
	set485out();
	delayms(WAITTIME485);
	a=sendSerial(m,n);
	while(!sent());
	set485in();
	return a;
}
uch recvModbus(uch data *m)
{
	return recvSerial(m);
}
uch* getContent(uch chAddr,uch data *buffer,uch Protocol)
{
	uch crcByte, szByte[3], *p;
	szByte[2]=0;
	if(Protocol==ASCII)
	{
	crcByte=0xb5-chAddr;
	buffer[0]=':';
	lmemcpy(buffer+1,dByte2Str(chAddr,szByte),2);
	buffer[3]=0;
	lstrcat(buffer,"0347000001");
	dByte2Str(crcByte,szByte);
	lstrcat(buffer,szByte);
	lstrcat(buffer,"\r\n");
	return buffer;
	}
	else
	{
	p=buffer;
	*p++=chAddr;
	*p++=0x03;
	*p++=0x0;
	*p++=0x2;
	forCrc16(buffer,4,p);
	return buffer;
	}
}
bit checkRecv(uch data *buffer,uch Protocol)
{
	uch tmp[7];

	if(Protocol==ASCII)
	{
	if(sumCheck(str2Ch(tmp,buffer+1),ASCIIREVFRAMELEN))return 0;
	lmemcpy(buffer,tmp,ASCIIREVFRAMELEN);
	return 1;//check ok
	}
	else
	{
	forCrc16(buffer,RTUREVFRAMELEN,buffer+RTUREVFRAMELEN);
	if(buffer[RTUREVFRAMELEN]==0 && buffer[RTUREVFRAMELEN+1]==0)return 1;//check ok
	return 0;
	}
}

uint getModbusData(uch data*buffer)
{
	union ASCData
	{
		uch b[5];
		struct stt
		{
			uch b3[3];
			uint intd;
		}s;
	} ad;
	lmemcpy(&(ad.b[0]),buffer,5);
	return ad.s.intd;
}

bit getMesureData(uch chAddr,uint *piIntData,uch Protocol)
{
	uch recvLen,sendLen,chTimes=0,chWaitTimes,chTmp;	
	uch buffer[MAXDATALEN+1];
	lmemset(buffer,0,MAXDATALEN+1);
	ledRvs();
	if(Protocol==ASCII)
	{
		recvLen=MODBUSASCIIRECVLEN;
		sendLen=MODBUSASCIISENDLEN;
	}
	else
	{
		recvLen=MODBUSRTURECVLEN;
		sendLen=MODBUSRTUSENDLEN;
	}
	do
	{
		do
		{
			getContent(chAddr,buffer,Protocol);
			chWaitTimes=0;
			sendModbus(buffer,sendLen);
			while((chTmp=recvModbus(buffer))<recvLen && chWaitTimes<WAITTIMES)chWaitTimes++;
			if(chTmp>=recvLen)break;
			chTimes++;
		}while(chTimes<REPEATTIMES);
		if(chTmp>=recvLen && checkRecv(buffer,Protocol))
		{
			*piIntData=getModbusData(buffer);
			return 1;		
		}
	}while(chTimes<REPEATTIMES);
	*piIntData=0;//if failed , the value will be clear to zero
	return 0;
}

