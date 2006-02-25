#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "define.h"
extern uch 	t250usct,		//for 10ms and 1s 
		   	delayct,		//for delay function
			t10msct,		//for 10ms 
			commuCt,		//for data output by P1
			uchDataIndex;	//for data output by P1
extern uch bFFirst;			//for data output by P1
extern union g_uniInstruData instruData;	//for measure data store

#endif
