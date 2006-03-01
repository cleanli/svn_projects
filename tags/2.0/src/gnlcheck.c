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
