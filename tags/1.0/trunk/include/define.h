#ifndef _DEFINE_H
#define _DEFINE_H

#include <AT89X51.H>

#define uch unsigned char
#define uint unsigned int
#define ON 0
#define OFF 1

//used to store the measure data of 9 measure device
union g_uniInstruData
{
	uch nuchByteData[20];
	uint uintData[9];
};

#endif
