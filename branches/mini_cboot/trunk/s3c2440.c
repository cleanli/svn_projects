/*s3c2440.c:define the related func code for s3c2440*/

#include "s3c2440.h"
/*
void s3c2440_serial_init(){
	GPHCON = 0xaaa0;
	GPHUP = 0x7ff;
	ULCON0 = 0x3;
	UCCON0 = 0x5;
	UBRDIV0 = 18;//set bautrate 9600bps
}
*/
void s3c2440_serial_send_byte(unsigned char c)
{
        while(!(USCON0 & 0x2));
        UTXH0 = c;
}

unsigned char s3c2440_serial_recv_byte()
{
        while(!(USCON0 & 0x1));
        return URXH0;
}

uint s3c2440_is_serial_recv()
{
	return (USCON0 & 0x1);
}

