#include "comdefine.h"
#include "gnlasc.h"
#include "gnlstring.h"

#include "gnlmath.h"

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
 /*
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
  
uint dByte2Int(uch hi,uch low)
{
	union uniIntBytes uniTmp;
	uniTmp.uchCh[0]=hi;
	uniTmp.uchCh[1]=low;
	return uniTmp.uiI;
}

void bytes2Str(uch *dest, uch *source, uch len)
{
	while(len-->0)
	{
		dByte2Str(*source++,dest);
		dest=dest+2;
	}
	*dest=0;
	return;
}	   */

void int2Str(uch* acTmp,uint iValue, struct tpIDS *sttIntDispSet)
{
	uch w,f,i,*p;
	uint v,base;
	BOOL bF=1;
	w=sttIntDispSet->cWidth;
	f=sttIntDispSet->cFill;
	v= iValue;
	p=acTmp;
	if(w<5)
	{
		base=IPower(10,w);
 	    i=v/base;
	    v=v-i*base;
		base/=10;
	}
	else
	{
		w=5;
		base=IPower(10,w-1);
	}
	while(w-->0)
	{
		i=v/base;
		v=v-i*base;
		base/=10;
		i+=0x30;
		if(i==0x30 && bF)i=f;
		else bF=0;
		*p++=i;
	}
	*p=0;
	if(bF)*(p-1)=0x30;
}
