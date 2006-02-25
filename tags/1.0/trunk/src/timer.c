#include "timer.h"
#include "hardintface.h"

void delayms(unsigned char n)//n<63
{
	uch m;
	m=n<<2;
	resetCounter();
	while(getCounter()<=m);
}

void delay50ms(uch n)
{
	uch m=0;
	while(m<n)
	{
		delayms(50);
		m++;
	}
}


