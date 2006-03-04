#include "comdefine.h"
#include "gnlserial.h"
#ifdef _DEBUG
void seeValue(uch * hint, uch value)
{
	uch ucTmp;
	sendString(hint);
	sendASCByte(value);
	sendString("\n\rAnykey continue...\n\r");
	while(!recvSerialByte(&ucTmp));
}
#endif
