/*s3c2440.c:define the related func code for s3c2440*/

#include "s3c2440.h"

/* low level nand read function */
int nand_read_ll(unsigned char *buf, unsigned long start_addr, int size)
{
        int i, j;

        if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK)) {
                return -1;      /* invalid alignment */
        }

        NAND_CHIP_ENABLE;

        for(i=start_addr; i < (start_addr + size);) {
                /* READ0 */
                NAND_CLEAR_RB;
                NFCMD = 0;

                /* Write Address */
                NFADDR = i & 0xff;
                NFADDR = (i >> 9) & 0xff;
                NFADDR = (i >> 17) & 0xff;
                NFADDR = (i >> 25) & 0xff;

                NAND_DETECT_RB;

                for(j=0; j < NAND_SECTOR_SIZE; j++, i++) {
                        *buf = (NFDATA & 0xff);
                        buf++;
                }
		if(!((i>>9) & 0x3f))
			s3c2440_serial_send_byte('>');
        }
        NAND_CHIP_DISABLE;
        return 0;
}
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

