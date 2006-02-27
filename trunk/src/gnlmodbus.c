#include "gnlmodbus.h"
#include "hardintface.h"
#include "gnltimer.h"
#include "gnlserial.h"
#include "gnlasc.h"
#include "gnlcheck.h"
#include "gnlstring.h"

uch RrecvModbus(uch *pBuf)
{
	static yyy;
	yyy++;
	if(yyy==1)lstrcpy (pBuf,"wwwwww");
	if(yyy==2)lstrcpy(pBuf, "uudu\n\r");
	return 6;
}
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
uch recvModbus(uch *m,uch maxLen)
{
	return recvSerial(m,maxLen);
}
/*
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
}*/
bit checkRecv(uch data *buffer, uch Protocol)
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
/*
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
*/
void recvASCIICom(uch * buffer, uch maxLen)
{
	uch * pBuf, uchRvLen, uchTtRvLen, leftLen;

//	lstrcpy(buffer,":0703010020D5\r\n");return;
	
	uchTtRvLen=0;
	leftLen=maxLen;
	pBuf=buffer;
	while(1)
	{
		while((uchRvLen=recvModbus(pBuf,leftLen))==0);
		if(uchRvLen==0)
		{
			continue;
		}
		uchTtRvLen+=uchRvLen;
		if(uchTtRvLen>=maxLen)//if buffer is full 
		{
			pBuf=buffer;
			leftLen=maxLen;
			uchTtRvLen=0;
			continue; 
		}
		else
		{			
			if(uchTtRvLen>1)
			{
				if(buffer[uchTtRvLen-2]=='\r' && buffer[uchTtRvLen-1]=='\n')
				{
					buffer[uchTtRvLen]=0;
					return;
				}
			}
			pBuf+=uchRvLen;
			leftLen-=uchRvLen;
		}
	}
}

void dealASCIICom(uch *response, uch *com, uch addr)
{
	uch i=0,tmp,dataLen;
	uch xdata *dataPtr;

	response[i]=0;

	//syntax check

	if (com[0]!=':')return;//first=':'

	tmp=lstrlen(com+1);
	if ( ( tmp & 0x1 ) == 0x1 ) return ;//length except first ":" can divided by 2

	str2Ch(com,com+1);//change ASC to bytes

	if(com[0]!=addr) return;//addr is local addr

	tmp/=2;//tmp is length, pcom is second byte
	if (sumCheck(com,tmp)!=0) return;//sum check should be ok
	
	if(com[1]==0x03)//read command
	{
		dataLen=com[2];
		dataPtr=dByte2Int(com[3],com[4]);
		//begin write data to com
		lmemcpy(com+3,dataPtr,dataLen);
		com[dataLen+3]=0xff-sumCheck(com,dataLen+3)+1;
		//write to response
		response[0]=':';
		bytes2Str(response+1,com,dataLen+4);
		lstrcat(response,"\r\n");
		return;
	}
	else
	{
		return;
	}
}
