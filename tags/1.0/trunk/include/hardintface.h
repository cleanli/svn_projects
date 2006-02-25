/********************************************************
 *			Name : hardintface.h						*
 *			Dscr : Hardware related function of MCU		*
 *			Auth : Litao								*
 *			Date : 2006.1.22							*
 ********************************************************/

#ifndef _HARD_INT_FACE_H
#define _HARD_INT_FACE_H
#include "define.h"

#define led P3_5
#define outen485 P3_7
#define outPort P1

//serial////////////////////////////////////////////////////////////////////////////
/***********************************************************************************
Func: Set the flag of serial byte has been sent
Para: None
Retn: NOne
***********************************************************************************/
void setSentFlag();

/***********************************************************************************
Func: Clear the flag of serial byte has been sent
Para: None
Retn: NOne
***********************************************************************************/
void clrSentFlag();

/***********************************************************************************
Func: Clear the flag of serial byte has been received
Para: None
Retn: NOne
***********************************************************************************/
void prepSerialRecv();

/*******************************************************
Func : Judge if has sent
Para : None
Retn : 1 if sent and 0 if not sent
*******************************************************/
bit sent();

/*******************************************************
Func : Judge if has sent
Para : None
Retn : 1 if sent and 0 if not sent
*******************************************************/
bit received();

/*******************************************************
Func : Send a byte by serial port
Para : uch - the byte to send
Retn : None
*******************************************************/
void send(uch a);

/*******************************************************
Func : Get a byte received by serial port
Para : None
Retn : The byte received
*******************************************************/
uch getRecv();

//timer////////////////////////////////////////////////////////////////////////////
/*******************************************************
Func : Reset the timer counter (every 250us crease)
Para : None
Retn : None
*******************************************************/
void resetCounter();

/*******************************************************
Func : Get the current value of the timer counter (every 250us crease)
Para : None
Retn : Counter value
*******************************************************/
uch getCounter();

//485/////////////////////////////////////////////////////////////////////
/*******************************************************
Func : Set 485 communication in send mode
Para : None
Retn : None
*******************************************************/
void set485out();

/*******************************************************
Func : Set 485 communication in receive mode
Para : None
Retn : None
*******************************************************/
void set485in();

//led///////////////////////////////////////////////////////////////////////
/*******************************************************
Func : Make the led on
Para : None
Retn : None
*******************************************************/
void ledOn();

/*******************************************************
Func : Make the led off
Para : None
Retn : None
*******************************************************/
void ledOff();

/*******************************************************
Func : Make the led change the status
Para : None
Retn : None
*******************************************************/
void ledRvs();


#endif
