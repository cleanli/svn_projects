#include "gnlserial.h"
//#include "hardintface.h"
#include "gnlstring.h"
#include "gnlasc.h"
#include "atomic.h"



extern xdata unsigned long time_sec;
static xdata unsigned long delayct;

//serial

void resetCounter()
{
	delayct=time_sec;
}
uch getCounter()
{
	return (uch)(time_sec-delayct);
}
//void setSentFlag()
//{
//	TI=1;
//}

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

uch received()
{
	return RI?1:0;
}

void send(uch a)
{
	SBUF=a;
}
/*
uch getRecv()
{
	return SBUF;
}
*/
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
/*
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
	*/
uch sendSerial(uch *m,uch n)
{
	uch a=0;
    


//    while(atomic_test_inc(&lock))
//        task_sleep(serial_sleep);
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
    //spin_unlock(&lock);
//    atomic_dec(&lock);
//	task_wake(serial_sleep);
	return a;
}

  /*
uch recvSerial(uch *m,uch maxlen)
{
	uch n=0;
	while(n<maxlen && (recvSerialByte(m++)))n++;
	return n;
}	 
 */
bit sendASCByte(uch n)
{
	uch cT[2];
	if(sendSerial(dByte2Str(n,cT),2)<2)return 0;
	return 1;
}	

void sendString(uch *c)
{
	//sendSerial(c,lstrlen(c));
    for ( ; *c; c++) {
        sendSerialByte(*c);
    }
}

void sendInt(int i)
{
    xdata uch buf[6];
	xdata struct tpIDS tmpS={5,' '};

	int2Str(buf,i,&tmpS);
	sendString(buf);
}
