#include "gnlmath.h"
#include "comdefine.h"

uint IPower(uch x,uch n)
{
	uint y=1;
	while(n-->0)
	{
		y*=x;
	}
	return y;
}
