#ifndef _GNL_STRING_H 
#define _GNL_STRING_H 

#include "comdefine.h"

	/*****************************************************************************************
     Function : Find the string length
     Parameter: uch* - string memory address
     Return   : String length
     *****************************************************************************************/
	 uch  lstrlen(uch*);

	/*****************************************************************************************
     Function : String copy
     Parameter: uch* - dest string address
	 			uch* - source string address
     Return   : dest string address
     *****************************************************************************************/
	 uch* lstrcpy(uch data*,uch*);

	 /*****************************************************************************************
     Function : Append the string
     Parameter: uch* - dest string address
	 			uch* - source string address
     Return   : dest string address
     *****************************************************************************************/
	 uch* lstrcat(uch data*,uch*);

	 /*****************************************************************************************
     Function : Memory copy
     Parameter: uch* - dest memory address
	 			uch* - source memory address
				uch  - number of copy memory
     Return   : dest memory address
     *****************************************************************************************/
	 uch* lmemcpy(uch data*d,uch*s,uch n);

	 /*****************************************************************************************
     Function : Memory set value
     Parameter: uch* - memory address
	 			uch  - value to set
				uch  - number of memory
     Return   : dest memory address
     *****************************************************************************************/
	 uch* lmemset(uch data*s,uch vl,uch n);

#endif



