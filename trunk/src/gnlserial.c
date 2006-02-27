#include "gnlserial.h"
#include "hardintface.h"
#include "gnlstring.h"


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