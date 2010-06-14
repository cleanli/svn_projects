#include "xmodem.h"
#include "type.h"
#include "debug.h"
#include "interface.h"

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
        uint i = 128, tmp = 0xa0000, repeat = 86;
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
		if(time_limit_recv_byte(0x90000, &recved) == 1)
			(*((void (*)())p))();
		return 0;
	}
	index++;
	goto start_recv_package;
}
