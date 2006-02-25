#ifndef _SERIAL_H
#define _SERIAL_H

#include "define.h"

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
bit recvSerialByte(uch data*);

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
Retn: Counts of received bytes
***********************************************************************************/
uch recvSerial(uch data*);

#endif
