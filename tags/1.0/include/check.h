#ifndef _CHECK_H
#define _CHECH_H
#include "define.h"
     /**************************************************************************
     Function : Compute the sum of mem
     Parameter: uch*  - The start address of the mem
                uch   - The number of mem byte to compute
     Return   : The sum
     **************************************************************************/
     uch sumCheck(uch*,uch);
    
     /*******************************************************************************
     Function : Compute the CRC16 code of mem
     Parameter: uch*  - The start address of the mem
                uch   - The number of mem byte to compute
                uch*  - The address the crc16 check code write to
     Return   : The address the crc16 check code write to
     **************************************************************************/      
     uch* forCrc16(uch *crcdata,uch len,uch *crccode);

#endif
