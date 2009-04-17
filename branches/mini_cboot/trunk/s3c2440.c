/*s3c2440.c:define the related func code for s3c2440*/

#include "s3c2440.h"
#include "debug.h"


//#define NAND_DEBUG

void nand_reset()
{
	uint tmp = 10;
	NFCONF = (7<<12)|(7<<8)|(7<<4)|(0<<0);
	NFCONT = (1<<4)|(0<<1)|(1<<0);
	NFSTAT = 0x4;
	NFCMD = 0xff;
	while(tmp--);
        NAND_CHIP_DISABLE;
}

/* low level nand read function */
int nand_read_ll(unsigned char *buf, unsigned long start_addr, int size)
{
        int i, j;
	
	lprint("Copy Command:membuf=%x, nandaddr=%x, size=%x\r\n", buf, start_addr, size);
        if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK)) {
                return -1;      /* invalid alignment */
        }
	if(!(NFSTAT&0x1)){
		lprint("nand flash may have some problem, quit!\r\n");
		return -1;
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

#define ERASE_BLOCK_ADDR_MASK (512 * 32 -1)
uint random_read_nand(uint spare, uint addr)
{
	uint tmd;
	if(spare) 
		tmd = 0x50;
	else if(addr & 0x100)
		tmd = 0x01;
	else	
		tmd = 0;
        NAND_CHIP_ENABLE;
        NAND_CLEAR_RB;
	NFCMD = tmd;
        NFADDR = (addr) & 0xff;
        NFADDR = (addr >> 9) & 0xff;
        NFADDR = (addr >> 17) & 0xff;
        NFADDR = (addr >> 25) & 0xff;
        NAND_DETECT_RB;
	tmd =  NFDATA & 0xff;
        NAND_CHIP_DISABLE;
	return tmd;
}
	
int is_marked_bad_block(uint addr)
{
	unsigned char a;
	uint i;

	addr &= ~ERASE_BLOCK_ADDR_MASK;
        if(!(NFSTAT&0x1)){
                lprint("nand flash may have some problem, quit!\r\n");
                return -1;
        }
	if(random_read_nand(1, addr+5) != 0xff || 
		random_read_nand(1, addr+5+512) != 0xff)
		return 1;

	return 0;	
}

#if 0
int is_marked_bad_block(uint addr)
{
	unsigned char a;
	uint i;

	addr &= ~ERASE_BLOCK_ADDR_MASK;
        if(!(NFSTAT&0x1)){
                lprint("nand flash may have some problem, quit!\r\n");
                return -1;
        }
        NAND_CHIP_ENABLE;
        NAND_CLEAR_RB;
	NFCMD = 0x50;
        NFADDR = (addr) & 0xff;
        NFADDR = (addr >> 9) & 0xff;
        NFADDR = (addr >> 17) & 0xff;
        NFADDR = (addr >> 25) & 0xff;
        NAND_DETECT_RB;
	i = 5;
	while(i--){
		a = NFDATA;
#ifdef NAND_DEBUG
		lprint("%x\r\n", a);
#endif  
	}
	a = NFDATA;
#ifdef NAND_DEBUG
	lprint("%x\r\n", a);
#endif  
	if(a != 0xff){
#ifdef NAND_DEBUG
		lprint("marded bad\r\n");
#endif  
		return 1;
	}
	i = 15;
	while(i--){
		a = NFDATA;
#ifdef NAND_DEBUG
		lprint("%x\r\n", a);
#endif  
	}
	a = NFDATA;
#ifdef NAND_DEBUG
	lprint("%x\r\n", a);
#endif  
	if(a != 0xff){
#ifdef NAND_DEBUG
		lprint("marded bad\r\n");
#endif  
		return 1;
	}
	i = 10;
	while(i--){
		a = NFDATA;
#ifdef NAND_DEBUG
		lprint("%x\r\n", a);
#endif  
	}
#ifdef NAND_DEBUG
	lprint("not marked bad\r\n");
#endif  
        NAND_CHIP_DISABLE;
	return 0;	
}
#endif /*if 0*/

int nand_erase_ll(uint addr)
{
        lprint("Erase Command:addr=%x\r\n", addr);
	if(addr & ERASE_BLOCK_ADDR_MASK){
		lprint("erase addr not correct!\r\n");
		return -1;
	}	
        if(!(NFSTAT&0x1)){
                lprint("nand flash may have some problem, quit!\r\n");
                return -1;
        }
/*
	if(is_marked_bad_block(addr)){
                lprint("block %x is bad block, quit!\r\n",addr);
                return -1;
	}
*/
        NAND_CHIP_ENABLE;
        NAND_CLEAR_RB;
	NFCMD = 0x60;
        NFADDR = (addr >> 9) & 0xff;
        NFADDR = (addr >> 17) & 0xff;
        NFADDR = (addr >> 25) & 0xff;
	NFCMD = 0xd0;
	while(!(NFSTAT & 0x1)){
#ifdef NAND_DEBUG
		lprint("%x\r\n", NFSTAT);
#endif	
	}
	NFCMD = 0x70;
	if(NFDATA & 0x1){
		lprint("erase failed! may get bad.\r\n");
		return -1;
	}
	lprint("erase successfully! \r\n");
        NAND_CHIP_DISABLE;
	return 0;
}

int nand_write_ll(unsigned char *buf, unsigned long start_addr, int size)
{
        int i, j;

        lprint("Write Command:membuf=%x, nandaddr=%x, size=%x\r\n", buf, start_addr, size);
        if ((start_addr & NAND_BLOCK_MASK) || (size & NAND_BLOCK_MASK)) {
                return -1;      /* invalid alignment */
        }
        if(!(NFSTAT&0x1)){
                lprint("nand flash may have some problem, quit!\r\n");
                return -1;
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

