#include "define.h"
#include "lstring.h"
uch lstrlen(uch*s)
{	
	uch i=0;
	while(*s++!=0)i++;
	return i;
}
uch * lstrcpy(uch data*d,uch*s)
{
	uch data*p=d,tmp;
	while((tmp=*s++)!=0)*d++=tmp;
	*d=0;
	return p;
}
uch * lstrcat(uch data*d,uch*s)
{
	uch data*p=d;
	p+=lstrlen(p);
	lstrcpy(p,s);
	return d;
}
uch* lmemcpy(uch data*d,uch*s,uch n)
{
	uch data*p=d;
	while(n-->0)*d++=*s++;
	return p;
}
uch* lmemset(uch data*s,uch vl,uch n)
{
	uch data*p=s;
	while(n-->0)*p++=vl;
	return s;
}
