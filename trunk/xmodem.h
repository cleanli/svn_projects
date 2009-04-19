#ifndef _XMODEM_H
#define _XMODEM_H
#include "type.h"

#define SOH 0x01
#define EOT 0x04
#define ACK 0x06
#define NACK 0x15
#define CAN 0x18

uint time_limit_recv_byte(uint limit, unsigned char * c);
uint xmodem_1k_recv(unsigned char *);
#endif
