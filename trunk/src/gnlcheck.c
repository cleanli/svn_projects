#include "comdefine.h"
#include "gnlcheck.h"

uch sumCheck(uch*b,uch n)
{
	uch sum=0;
	while(n-->0)
	{
		sum+=*b++;
	}
	return sum;
}
/*
uch* forCrc16(uch *crcdata,uch len,uch *crccode)
{
	union uniInt
	{
		uint crcm;
		uch chCh[2];
	}abc;	
	unsigned int m;
	uch k=0,j=0;

	abc.crcm=0xffff;
	while(len>0)
	{
		m=crcdata[k];
		abc.crcm^=m;
		for (j=0;j<8;j++)
		{
			m=0x0001;
			m&=abc.crcm;
			abc.crcm>>=1;
			if(m!=0)
			{
				abc.crcm^=0xa001;
			}
		}
		k++;
		len--;
	}
#ifndef _BIGMODE	
	crccode[1]=abc.chCh[0];
	crccode[0]=abc.chCh[1];
#else
	crccode[0]=abc.chCh[0];
	crccode[1]=abc.chCh[1];
#endif	
	return crccode;
}	  */  
