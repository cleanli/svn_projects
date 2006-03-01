#include "gnlmodbus.h"
#include "hardintface.h"
#include "gnltimer.h"
#include "gnlserial.h"
#include "gnlasc.h"
#include "gnlcheck.h"
#include "gnlstring.h"

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

void recvASCIICom(uch * buffer, uch maxLen)
{
	uch * pBuf, uchRvLen, uchTtRvLen, leftLen;

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
	uch tmp,dataLen;
	uch xdata *dataPtr;

	response[0]=0;

	//syntax check

	if (com[0]!=':')return;//first=':'

	tmp=lstrlen(com+1);
	if ( ( tmp & 0x1 ) == 0x1 ) return ;//length except first ":" can divided by 2

	str2Ch(com,com+1);//change ASC to bytes

	if(com[0]!=addr) return;//addr is local addr

	tmp/=2;//tmp is length, pcom is second byte
	if (sumCheck(com,tmp)!=0) return;//sum check should be ok
	dataLen=com[2];
	dataPtr=dByte2Int(com[3],com[4]);
	
	if(com[1]==0x03)//  read command  
	{//  ":" + LocalAddr + "03" + DataLength + DataAddr(Hi) + DataAddr(Low) + SumCheck + <CR> + <LF>
		if(tmp!=7)return;//length not right
		//begin write data to com
		lmemcpy(com+3,dataPtr,dataLen);
		*dataPtr=0x44;
		com[dataLen+3]=0xff-sumCheck(com,dataLen+3)+1;
		//write to response
		response[0]=':';
		bytes2Str(response+1,com,dataLen+4);
		lstrcat(response,"\r\n");
		return;
	}
	else if (com[1]==0x06)//write
	{//  ":" + LocalAddr + "06" + DataLength + DataAddr(Hi) + DataAddr(Low) + Data ... + SumCheck + <CR> + <LF>
		if(tmp!=(7+dataLen))return;
		//begin write data to local memory
		lmemcpy(dataPtr,com+5,dataLen);
		com[5]=0xff-sumCheck(com,5)+1;
		//write to response
		response[0]=':';
		bytes2Str(response+1,com,6);
		lstrcat(response,"\r\n");
		return;		
	}
	else
	{
		return;
	}
}
