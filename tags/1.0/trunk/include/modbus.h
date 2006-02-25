#ifndef _MODBUS_H
#define _MODBUS_H

#include "define.h"

//The wait time to send in 'ms' after set to send mode
#define WAITTIME485 1

//The times of execute recvModbus(char*)
#define WAITTIMES 100

//Times of send data require frame
#define REPEATTIMES 2

//Buffer length
#define MAXDATALEN 17

//Length of data require frame of ASCII
#define MODBUSASCIISENDLEN 17

//Length of response data frame of ASCII
#define MODBUSASCIIRECVLEN 15

//Length of data require frame of RTU
#define MODBUSRTUSENDLEN 6

//Length of response data frame of RTU
#define MODBUSRTURECVLEN 7

//Length of received ASCII frame
#define ASCIIREVFRAMELEN 6

//Length of received RTU frame
#define RTUREVFRAMELEN 7

//The parameter of Protocol
#define ASCII 0
#define RTU 1

/***********************************************************************************
Func: Send memory byte to 485 bus
Para: uch* - memory address
	  uch  - number of bytes
Retn: number of bytes sent successfully
***********************************************************************************/
uch sendModbus(uch*,uch);

/***********************************************************************************
Func: Receive memory byte from 485 bus
Para: uch* - memory address
Retn: number of bytes received
***********************************************************************************/
uch recvModbus(uch data*);

/***********************************************************************************
Func: Get content of data require frame
Para: uch  - Modbus address
	  uch* - Address content write to
	  uch  - Modbus Protocol, ASCII or RTU
Retn: Start address of the content
***********************************************************************************/
uch* getContent(uch chAddr,uch data * buffer,uch Protocol);

/***********************************************************************************
Func: Check the mocbus received data
Para: uch* - Data address
	  uch  - Modbus Protocol, ASCII or RTU
Retn: 1 if OK and 0 if Error
***********************************************************************************/
bit checkRecv(uch data *buffer,uch Protocol);

/***********************************************************************************
Func: Get the data from the mocbus received frame
Para: uch* - Received buffer address
Retn: Data in integer
***********************************************************************************/
uint getModbusData(uch data *buffer);

/***********************************************************************************
Func: Get the specified Modbus address Device's data
Para: uint* - Returned data's address 
	  uch  - Modbus Protocol, ASCII or RTU
Retn: 1 if OK and 0 if Error
***********************************************************************************/
bit getMesureData(uch chAddr,uint *piIntData,uch Protocol);

#endif





