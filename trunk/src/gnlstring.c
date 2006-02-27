#include "comdefine.h"
#include "gnlstring.h"
uch lstrlen(uch*s)
{	
	uch i=0;
	while(*s++!=0)i++;
	return i;
}
uch * lstrcpy(uch *d,uch*s)
{
	uch *p=d,tmp;
	while((tmp=*s++)!=0)*d++=tmp;
	*d=0;
	return p;
}
uch * lstrcat(uch *d,uch*s)
{
	uch *p=d;
	p+=lstrlen(p);
	lstrcpy(p,s);
	return d;
}
uch* lmemcpy(uch *d,uch*s,uch n)
{
	uch *p=d;
	while(n-->0)*d++=*s++;
	return p;
}
uch* lmemset(uch *s,uch vl,uch n)
{
	uch *p=s;
	while(n-->0)*p++=vl;
	return s;
}