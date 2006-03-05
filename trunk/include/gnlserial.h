#ifndef _GNL_SERIAL_H
#define _GNL_SERIAL_H

#include "comdefine.h"

#define WAITTIME 16 //4ms, the wait time when receive data from serial port

/***********************************************************************************
Func: Send by serial a byte
Para: uch - Byte to send
Retn: 1 if OK and 0 if Error
***********************************************************************************/
bit sendSerialByte(uch);

/***********************************************************************************
Func: Receive a byte by serial
Para: uch * - Address to receive the byte
Retn: 1 if OK and 0 if Error
***********************************************************************************/
bit recvSerialByte(uch *);

/***********************************************************************************
Func: Send memory by serial 
Para: uch * - memory start address
      uch   - length of memory to send
Retn: Counts of sent bytes
***********************************************************************************/
uch sendSerial(uch *,uch);

/***********************************************************************************
Func: Receive bytes by serial 
Para: uch * - memory start address
	  uch - max len of buffer
Retn: Counts of received bytes
***********************************************************************************/
uch recvSerial(uch* , uch);

/***********************************************************************************
Func: Send bytes by ASC 
Para: uch - byte to send
Retn: 1 if ok and 0 if failed
***********************************************************************************/
bit sendASCByte(uch n);

/***********************************************************************************
Func: Send string
Para: uch* - mem string addr
Retn: none
***********************************************************************************/
void sendString(uch *c);

#endif
