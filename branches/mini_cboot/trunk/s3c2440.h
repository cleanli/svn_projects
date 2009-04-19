/*s3c2440.h:define the related func & header for s3c2440*/

#ifndef _S3C2440_H
#define _S3C2440_H
#include "type.h"

/*Serial reg*/
#define USCON0 *(volatile unsigned long *)0x50000010
#define UTXH0 *(volatile unsigned long *)0x50000020
#define URXH0 *(volatile unsigned long *)0x50000024

void s3c2440_serial_init();
void s3c2440_serial_send_byte(unsigned char c);
unsigned char s3c2440_serial_recv_byte();
uint s3c2440_is_serial_recv();

#endif /*_S3C2440_H*/
