#include "cmd.h"
#include "debug.h"
#include "version.h"
#include "print.h"
#include "xmodem.h"

static unsigned char cmd_buf[COM_MAX_LEN];
uint cmd_buf_p = 0;

static const struct command cmd_list[]=
{
    {"gfbs",get_file_by_serial},
    {"go",go},
    {"help",print_help},
    {"pm",print_mem},
    {"r",read_mem},
    {"w",write_mem},
    {NULL, NULL},
};
static uint * mrw_addr = 0x0;

void go(unsigned char *para)
{
	(*((void (*)())mrw_addr))();
}

void get_file_by_serial(unsigned char *para)
{
	xmodem_1k_recv((unsigned char*)mrw_addr);

/*
	uint i = 1028, tmp = 0x20000, repeat = 36;
	unsigned char *c = (unsigned char*const)0xbf4;

	lprint("\r\nNow start(*0xbf4 = 0 will be test):\r\n\r\n");
	if(*c)
		con_send(0x15);
	else
		lprint("\r\nTest, time is up!\r\n");
	while(tmp--)
		if(is_con_recv())
			break;
	while(i--)
		*c++ = con_recv();
	con_send(0x06);
	*c++ = con_recv();
	con_send(0x06);
*/
}

void print_help(unsigned char *para)
{
    uint i = 0;
    lprint("Available cmd is:\r\n");
    while(1){
            if(cmd_list[i].cmd_name == NULL)
                    break;
	    lprint("--%s\r\n", cmd_list[i].cmd_name);
            i++;
    }
}

uint asc_to_hex(unsigned char c)
{
	uint v;

	if(c >= '0' && c <= '9')
		return c - '0';	
	if(c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if(c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return 0;
}

uint get_howmany_para(unsigned char *s)
{
	uint tmp = 0;
	while(1){
		while(*s == ' ')
			s++;
		if(*s)
			tmp++;
		while(*s != ' ' && *s)
			s++;
		if(!*s)
			return tmp;
	}
}

unsigned char * str_to_hex(unsigned char *s, uint * result)
{
	uint  i = 0;

	*result = 0;
	while(*s == ' ')s++;
	for(i=0;i<8;i++){
		if(*s == ' ' || *s == 0)
			break;
		*result = *result*16 + asc_to_hex(*s++);
	}
	while(*s == ' ')s++;
	return s;
}

void print_mem(unsigned char *p)
{
    uint length = 0x80, tmp, i;
    unsigned char *cp;

    tmp = get_howmany_para(p);
    if( tmp > 1)
        goto error;
    if(tmp == 0)
        goto print;
    str_to_hex(p, &length);
print:
    cp = (unsigned char *)mrw_addr;
    lprint("Start print 0x%x mem content @%x:\r\n", length, (uint)mrw_addr);
    while(length){
	lprint("\r\n");
	for(i=0;i<8;i++){
		length--;
		lprint("%x\t", *cp++);
	}
    }
    lprint("\r\nPrint end @%x.\r\n", (uint)mrw_addr);

    return;

error:
    lprint("Error para!\r\npm [length](default 0x80 if no this argu)\r\n");

}

void write_mem(unsigned char *p)
{
    uint value, tmp;

    tmp = get_howmany_para(p);
    if(tmp == 0 || tmp > 2)
	goto error;
    p = str_to_hex(p, &value);
    if(tmp == 1)
        goto write;
    str_to_hex(p, (uint*)&mrw_addr);
    mrw_addr = (uint*)((uint)mrw_addr & 0xfffffffc);
write:
    *(uint*)mrw_addr = value;
    lprint("Write 0x%x to memory 0x%x done!\r\n",value,mrw_addr);
    return;

error:
    lprint("Error para!\r\nw (hex addr) [(hex addr)](last addr if no this argu)\r\n");

}


void read_mem(unsigned char *p)
{
    uint value, tmp;

    tmp = get_howmany_para(p);
    if( tmp > 1)
	goto error;
    if(tmp == 0)
    	goto read;
    str_to_hex(p, (uint*)&mrw_addr);
    mrw_addr = (uint*)((uint)mrw_addr & 0xfffffffc);
read:
    value = *(uint*)mrw_addr;
    lprint("Read 0x%x at memory 0x%x\r\n",value,mrw_addr);

    return;

error:
    lprint("Error para!\r\nr [(hex addr)](last addr if no this argu)\r\n");

}


unsigned char* lmemcpy(unsigned char *d,unsigned char*s,uint n)
{
	unsigned char *p=d;
	while(n--)*d++=*s++;
	return p;
}

void lmemset(unsigned char *d,unsigned char v,unsigned int n)
{
	while(n--)*d++=v;
}

void handle_cmd()
{
    unsigned char i = 0, *p_cmd, *p_buf;

    lprint("\r\n");
    if(!cmd_buf[0])
	return;
    while(1){
	    if(cmd_list[i].cmd_name == NULL)
		    break;
	    p_cmd=cmd_list[i].cmd_name;
	    p_buf=cmd_buf;
	    while(*p_cmd){
		    if(*p_buf != *p_cmd)
			    break;
		    p_buf++;
		    p_cmd++;
	    }
	    if(!(*p_cmd) && (*p_buf == ' ' || !(*p_buf))){
            	    cmd_list[i].cmd_fun(p_buf);
            	    return;
       	    }
	    i++;
    }
    lprint("Unknow command:%s\r\n",cmd_buf);
}

void run_clean_boot()
{
	unsigned char c;
	
	mrw_addr = 0;
	lprint("\r\n\r\nHello, this is clean_boot v%s.\r\n", CLEAN_BOOT_VERSION);
	lmemset(cmd_buf, 0, COM_MAX_LEN);
	cmd_buf_p = 0;
	lprint("\r\nCleanBoot@%s>", PLATFORM);
	while(1){
		if((c = con_recv()) != ENTER_CHAR){
			if(cmd_buf_p < (COM_MAX_LEN - 1)){
				cmd_buf[cmd_buf_p++] = c;
				con_send(c);
			}
		}
		else{
			handle_cmd();
			lmemset(cmd_buf, 0, COM_MAX_LEN);
			cmd_buf_p = 0;
			lprint("\r\nCleanBoot@%s>", PLATFORM);
		}
	}
}
