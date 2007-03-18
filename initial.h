			/********************************************************
			 *			Name : global.h	        					*
			 *			Dscr : define global variable				*
			 *			Auth : Litao								*
			 *			Date : 2006.2.28							*
			 ********************************************************/

#ifndef _INITIAL_H
#define _INITIAL_H

#include "comdefine.h"

#define TIMERCONST 1085
#define MAXBOLTRATE 57600
uch getTmrSet(uint interval);
uch getSrlSet(uint btv);
void chipInitial();

#endif
