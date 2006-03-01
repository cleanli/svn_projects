#ifndef _GNL_ASC_H
#define _GNL_ASC_H

#include "comdefine.h"

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
     Return   : The byte address of result
     *****************************************************************************************/
	 uch *str2Ch(uch *result, uch *buffer);

	 /*****************************************************************************************
     Function : Convert a two byte to unsigned int
                For example: 0x34,0x2b --> 0x342b
     Parameter: uch - high byte
				uch - low byte
     Return   : result uint
     *****************************************************************************************/
	 uint dByte2Int(uch,uch);

	 /*****************************************************************************************
     Function : Convert a memory bytes to ASC string
                For example: 0xa8,0x77,0x78, --> "A87778F"
     Parameter: uch* - result address
				uch* - The mem address to be convert
     Return   : None
     *****************************************************************************************/
	 void bytes2Str(uch *dest, uch *source, uch len);

#endif

