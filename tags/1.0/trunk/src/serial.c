#include "serial.h"
#include "hardintface.h"


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
bit recvSerialByte(uch data * n)
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


uch recvSerial(uch data *m)
{
	uch n=0;
	prepSerialRecv();
	while((recvSerialByte(m++)))n++;
	return n;
}
