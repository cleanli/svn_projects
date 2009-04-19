#include "xmodem.h"
#include "type.h"
#include "interface.h"

uint crc16(unsigned char *c, uint len)
{
	uint crc16_check = 0, j;

	while(len--)
	{
		crc16_check = crc16_check ^ (uint)*c++ << 8;
		for (j=0;j<8;j++)
		{
			if(crc16_check & 0x8000)
				crc16_check = crc16_check << 1 ^ 0x1021;
			else
				crc16_check <<= 1;
		}
	}
	return crc16_check & 0xffff;
}

uint time_limit_recv_byte(uint limit, unsigned char * c)
{
	while(limit--){
		if(is_con_recv()){
			*c = con_recv();
			return 0;
		}
	}
	return 1;
}

uint xmodem_1k_recv(unsigned char*p)
{
        uint i = 128, tmp = 0xa0000, repeat = 66;
	unsigned char recved = 0, index = 1, check_sum = 0, *c;

	c = p;
	do{
                con_send(0x15);
		if(time_limit_recv_byte(0x80000, &recved) == 0)
			break;
	}while(repeat--);
	if(!repeat){
		con_send(CAN);
		return 1;
	}
start_recv_package:
	if(recved != SOH){
		con_send(CAN);
		return 2;
	}
	if(time_limit_recv_byte(0x20000, &recved) == 1){
		con_send(CAN);
		return 2;
	}
	if(recved != index){
		con_send(CAN);
		return 2;
	}
	if(time_limit_recv_byte(0x20000, &recved) == 1){
		con_send(CAN);
		return 2;
	}
	if((recved + index) != 0xff){
		con_send(CAN);
		return 2;
	}
	i = 128;
	check_sum = 0;
	while(i--){
		if(time_limit_recv_byte(0x20000, &recved) == 1){
			con_send(CAN);
			return 2;
		}
		*c++ = recved;
		check_sum += recved;
	}
	if(time_limit_recv_byte(0x20000, &recved) == 1){
		con_send(CAN);
		return 2;
	}
	if(recved != check_sum){
		con_send(CAN);
		return 2;
	}
	con_send(ACK);
	if(time_limit_recv_byte(0x20000, &recved) == 1){
		con_send(CAN);
		return 2;
	}
	if(recved == EOT){
		con_send(ACK);
		return 0;
	}
	index++;
	goto start_recv_package;
}

uint xmodem_1k_send(unsigned char*c, uint len)
{
        uint i = 128, check_way = 0; /*0-crc16 1-checksum*/
	unsigned char tmpchar = 0, index = 1, check_sum = 0, *clast;
	uint check_crc16;

	lprint("Please get the receiver ready!\r\n");
	while(1){
		tmpchar = con_recv();
		if(tmpchar == 'C'){
			break;
		}
		if(tmpchar == NACK){
			check_way = 1;
			break;
		}
	}
start_send_package:
	if(index > len){
		con_send(EOT);
		con_recv();
		time_limit_recv_byte(0x20000, &tmpchar);
		return 0;
	}
	con_send(SOH);
	con_send(index);
	con_send(~index & 0xff);
	i = 128;
	check_sum = 0;
	if(!check_way)
		check_crc16 = crc16(c, 128);
	clast = c;
	while(i--){
		tmpchar = *c++;
		con_send(tmpchar);
		check_sum += tmpchar;
	}
	if(check_way)
		con_send(check_sum);
	else{
		//con_send(0xb1);
		//con_send(0x47);
		con_send((check_crc16>>8) & 0xff);
		con_send(check_crc16 & 0xff);
	}	
	tmpchar = con_recv();
	if(tmpchar == ACK)
		index++;
	else if(tmpchar == NACK)
		c = clast;
	else
		while(1);
	goto start_send_package;
}

