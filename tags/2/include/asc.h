#ifndef _ASC_H
#define _ASC_H

#include "define.h"
     /*****************************************************************************************
     Function : Convert the byte to string in Hex
     Parameter: uch   - The byte to be convert
                uch* - The result will be put in two byte this uch pointer point to
     Return   : The pointer of first result byte
     *****************************************************************************************/
     uch* dByte2Str(uch,uch*);  

     /*****************************************************************************************
     Function : Convert low four bit of the byte to ASC in Hex
                For example: xxxx0110 --> '6' xxxx1000--> '8' xxxx1110 --> 'E'
     Parameter: uch   - The byte to be convert
     Return   : The result byte
     *****************************************************************************************/
     uch sByte2Asc(uch b);

     /*****************************************************************************************
     Function : Convert an ASC byte to byte data
                For example: "A" --> 0xa   "9" --> 0x9    "p" -- > 0x0    "a" --> 0x0
     Parameter: uch* - The ASC byte address to be convert
     Return   : The result byte
     *****************************************************************************************/
     uch sASC2Byte(uch*s);

     /*****************************************************************************************
     Function : Convert two ASC byte to byte data
                For example: "A8" --> 0xa8    "89" --> 0x89     "u9" --> 0x09
     Parameter: uch* - The two ASC bytes address to be convert
     Return   : The result byte
     *****************************************************************************************/    
     uch dASC2Byte(uch*s);    
     
     /*****************************************************************************************
     Function : Convert a string to byte data
                For example: "A87778F" --> 0xa8,0x77,0x78,
     Parameter: uch* - result address
				uch* - The string address to be convert
     Return   : The string address of result
     *****************************************************************************************/
	 uch *str2Ch(uch *result, uch *buffer);

#endif


