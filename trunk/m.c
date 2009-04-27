#include "cmd.h"
#include "print.h"
#include "cs8900.h"

const unsigned char cs8900_mac[]={0x00, 0x43, 0x33, 0x2f, 0xde, 0x22};

void main()
{
	random_init();
	cs8900_init(cs8900_mac);
	run_clean_boot();
}
