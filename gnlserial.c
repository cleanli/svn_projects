#include "gnlserial.h"
//#include "hardintface.h"
#include "gnlstring.h"
#include "gnlasc.h"
static uch delayct;
//serial

void resetCounter()
{
	delayct=0;
}
uch getCounter()
{
	return delayct;
}
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

bit sendSerialByte(uch n)
{
	resetCounter();
	while(!sent() && getCounter()<WAITTIME);
	if(sent())
	{
		clrSentFlag();
		send(n);
		return 1;
	}
	else
	{
		return 0;
	}
}

bit recvSerialByte(uch * n)
{
	resetCounter();
	while(!received() && getCounter()<WAITTIME);
	if(received())
	{
		prepSerialRecv();
		*n=getRecv();
		return 1;
	}
	else
	{
		return 0;
	}
}

uch sendSerial(uch *m,uch n)
{
	uch a=0;
	while(n-->0)
	{
		if(!(sendSerialByte(*m++)))
		{
			break;
		}
		else
		{
			a++;
		}
	}
	return a;
}


uch recvSerial(uch *m,uch maxlen)
{
	uch n=0;
	while(n<maxlen && (recvSerialByte(m++)))n++;
	return n;
}

bit sendASCByte(uch n)
{
	uch cT[2];
	if(sendSerial(dByte2Str(n,cT),2)<2)return 0;
	return 1;
}

void sendString(uch *c)
{
	sendSerial(c,lstrlen(c));
}

void sendInt(int i)
{
    uch buf[6];
	struct tpIDS tmpS={5,' '};

	int2Str(buf,i,&tmpS);
	sendString(buf);
}
