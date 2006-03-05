#ifndef _COM_DEFINE_H
#define _COM_DEFINE_H

#include <AT89X51.H>

//#define _DEBUG

#define uch unsigned char
#define uint unsigned int
#define ULONG unsigned long int
#define BOOL bit

union uniIntBytes{
	uint uiI;
	uch uchCh[2];
};

	struct tpIDS
	{
		uch cWidth;
		uch cFill;
	};


#endif
