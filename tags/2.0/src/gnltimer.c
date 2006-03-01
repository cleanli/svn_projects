#include "gnltimer.h"
#include "hardintface.h"

void delayms(unsigned char n)//n<63
{
	uch m;
	m=n<<2;
	resetCounter();
	while(getCounter()<=m);
}
