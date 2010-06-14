#include "cmd.h"
#include "debug.h"
#include "version.h"
#include "print.h"
#include "xmodem.h"

static unsigned char cmd_buf[COM_MAX_LEN] = "";
static uint cmd_buf_p = COM_MAX_LEN;

static const struct command cmd_list[]=
{
    {"go",go},
    {"help",print_help},
    {"nandwb",nandwb},
    {"nandcopy",nandcopy},
    {"pm",print_mem},
    {"r",read_mem},
    {"w",write_mem},
    {"xmodem",get_file_by_serial},
    {NULL, NULL},
};
static uint * mrw_addr;

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

	lprint("\nNow start(*0xbf4 = 0 will be test):\n\n");
	if(*c)
		con_send(0x15);
	else
		lprint("\nTest, time is up!\n");
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
    lprint("Cmd:\n");
    while(1){
            if(cmd_list[i].cmd_name == NULL)
                    break;
	    lprint("--%s\n", cmd_list[i].cmd_name);
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
    while(length){
	lprint("\n%x: ", (uint)cp);
	for(i=0;i<16;i++){
		length--;
		print_ch(*cp++);
		con_send(i == 7 ? '-':' ');
	}
    }
    con_send('\n');

    return;

error:
    lprint("Err!\npm [length](0x80 default)\n");

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
    lprint("Write 0x%x@0x%x\n",value,mrw_addr);
    return;

error:
    lprint("Err!\nw (hexaddr) [(hexaddr)](last addr default)\n");

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
    lprint("Read 0x%x at memory 0x%x\n",value,mrw_addr);

    return;

error:
    lprint("Err!\nr [(hexaddr)](last addr default)\n");

}

void nandcopy(unsigned char *p)
{
    uint addr, tmp, size;

    tmp = get_howmany_para(p);
    if(tmp != 2)
        goto error;
    p = str_to_hex(p, &addr);
    p = str_to_hex(p, &size);

    if(nand_read_ll(mrw_addr, addr, size) == 0)
    	lprint("successfully\n");
    return;

error:
    lprint("Err!\nnandcopy hexaddr hexsize\n");

}

void nandwb(unsigned char *p)
{
    uint addr, tmp, c;

    tmp = get_howmany_para(p);
    if(tmp == 1)
        goto error;
    p = str_to_hex(p, &addr);
    tmp--;
    while(tmp--){
	    p = str_to_hex(p, &c);
	    c &= 0xff;
	    if(random_write_nand((unsigned char)c, addr) != 0){
		lprint("failed\n");
		return;
	    }
	    addr++;
    }
    lprint("ok\n");
    return;

error:
    lprint("Err!\nnandwb hexaddr hexchar...\n");

}

/*
unsigned char* lmemcpy(unsigned char *d,unsigned char*s,uint n)
{
	unsigned char *p=d;
	while(n--)*d++=*s++;
	return p;
}
*/

void lmemset(unsigned char *d,unsigned char v,unsigned int n)
{
	while(n--)*d++=v;
}

void handle_cmd()
{
    unsigned char i = 0, *p_cmd, *p_buf;

    lprint("\n");
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
    lprint("Unknow cmd:%s\n",cmd_buf);
}

uint time_limit_recv_byte(uint limit, unsigned char * c);
void run_clean_boot()
{
	unsigned char c;
	
	mrw_addr = (uint*)0x30000000;
	lprint("\n\nMini_clean_boot v%s.\nAnykey stop auto load file\n", CLEAN_BOOT_VERSION);
	xmodem_1k_recv((unsigned char*)mrw_addr);
	lmemset(cmd_buf, 0, COM_MAX_LEN);
	cmd_buf_p = 0;
	lprint("\nCleanBoot>");
	
	while(1){
		c = con_recv();
		if(c == ENTER_CHAR || c == 0x1b || c== 0x03){
			if(c == ENTER_CHAR)
				handle_cmd();
			lmemset(cmd_buf, 0, COM_MAX_LEN);
			cmd_buf_p = 0;
			lprint("\nCleanBoot>");
		}
		else if(c == 0x08){
			if(!cmd_buf_p)
				continue;
                        cmd_buf[--cmd_buf_p] = 0;
                        print_string("\b \b");
		}
		else{
			if(cmd_buf_p < (COM_MAX_LEN - 1)){
				cmd_buf[cmd_buf_p++] = c;
				con_send(c);
			}
		}
	}
}
