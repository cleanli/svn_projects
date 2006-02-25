#include "define.h"
#include "asc.h"
#include "lstring.h"
uch sByte2Asc(uch b)
{
	b&=0xf;
	b+=0x30;
	if(b>0x39)b+=7;
	return b;
}

uch* dByte2Str(uch b,uch*s)
{
	*(s+1)=sByte2Asc(b);
	b>>=4;
	*s=sByte2Asc(b);
	return s;
}

uch sASC2Byte(uch*s)
{
	uch tmp;
	tmp=*s;
	if(tmp>='A' && tmp<='F')
	{
		return (tmp-0x37);
	}
	else if(tmp>='0' && tmp<='9')
	{
		return (tmp-0x30);
	}
	else
	{
		return 0;
	}
}

uch dASC2Byte(uch*s)
{
	uch a=0;
	a+=sASC2Byte(s);
	a<<=4;
	a+=sASC2Byte(s+1);
	return a;
}

uch *str2Ch(uch *result, uch * buffer)
{
	uch l,i=0;
	uch*p=result;
	l=lstrlen(buffer)/2;
	while(l-->0)
	{
		result[i]=dASC2Byte(buffer+i*2);
		i++;		
	}
	return p;
}

