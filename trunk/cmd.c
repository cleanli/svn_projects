#include "cmd.h"
#include "debug.h"
#include "version.h"
#include "print.h"
#include "xmodem.h"

static uint get_howmany_para(unsigned char *s);
static unsigned char * str_to_hex(unsigned char *s, uint * result);
static unsigned char cmd_buf[COM_MAX_LEN];
uint cmd_buf_p = 0;

static const struct command cmd_list[]=
{
    {"gfbs",get_file_by_serial,"get file by serial"},
    {"go",go,"jump to ram specified addr to go"},
    {"help",print_help,"help message"},
    {"nandcp",nandcp, "copy nand data to ram specified addr"},
    {"nander",nander, "erase nand"},
    {"nandr",nandr,"random read nand data"},
    {"nandspr",nandspr,"random read nand spare data"},
    {"ndbb",ndbb,"check if one nand block is marked bad"},
    {"ndchkbb",ndchkbb,"scan all flash marked bad block"},
    {"pfbs",put_file_by_serial,"put file by serial"},
    {"pm",pm,"print memory content"},
    {"r",read_mem,"read mem, can set specified addr for other cmd"},
    {"w",write_mem,"write mem, also can set mem addr"},
    {NULL, NULL, NULL},
};
static uint * mrw_addr = 0x0;

void ndchkbb(unsigned char *para)
{
	uint i, addr, marked_bad = 0, addr_list[16];
	lprint("I will check all block(0 - 0x%x) on current K9f1208 and find out all marked bad block, please wait!\r\n", 0x4000 * 4096);
	i = 4096;
	addr = 0;
	while(i--){
		if(is_marked_bad_block(addr)){
			addr_list[marked_bad++] = addr;
			con_send('X');
		}
		else
			con_send('O');
		addr += 0x4000;
	}	
	lprint("\r\ncheck over! %x blocks bad.\r\n", marked_bad);
	i = 0;
	while(marked_bad--)
		lprint("%x\r\n", addr_list[i++]); 
}

void go(unsigned char *para)
{
	lprint("This will go at the addr you just used with the 'r' cmd. Any problem please check!\r\n");
	(*((void (*)())mrw_addr))();
}

void put_file_by_serial(unsigned char *p)
{
    uint length = 0x80, tmp, i;

    tmp = get_howmany_para(p);
    if( tmp > 1)
        goto error;
    if(tmp == 0)
        goto modemsend;
    str_to_hex(p, &length);
    length >>= 7;
    if(!length)
	goto error;
modemsend:
	xmodem_1k_send((unsigned char*)mrw_addr, length);
	return;
error:
    lprint("Error para!\r\npfbs [length(0x80bytes x n)](default 0x80(16k) if no this argu)\r\n");
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
    lprint("Clean Boot V%s\r\nAvailable cmd is:\r\n", CLEAN_BOOT_VERSION);
    while(1){
            if(cmd_list[i].cmd_name == NULL)
                    break;
	    lprint("--%s\r\n\t%s\r\n", cmd_list[i].cmd_name, cmd_list[i].cmd_des);
            i++;
    }
    lprint("'r' is a special command, if a address followed, it will be set as a memory base for many other command, such as 'pm', and so on\r\nESC will cancel current command\r\n");
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

void print_mem(unsigned char *cp, uint length)
{
    uint i;

    cp = (unsigned char *)((uint)cp & (~0x3));
    lprint("Start print 0x%x mem content @%x:\r\n", length, (uint)cp);
    while(length){
	lprint("\r\n");
	for(i=0;i<8;i++){
		length--;
		lprint("%x\t", *cp++);
	}
    }
    lprint("\r\nPrint end @%x.\r\n", (uint)cp);
}

void pm(unsigned char *p)
{
    uint length = 0x80, tmp, i;

    tmp = get_howmany_para(p);
    if( tmp > 1)
        goto error;
    if(tmp == 0)
        goto print;
    str_to_hex(p, &length);
print:
    print_mem((unsigned char*)mrw_addr, length);
    return;

error:
    lprint("Error para!\r\npm [length](default 0x80 if no this argu)\r\n");

}


void ndbb(unsigned char *p)
{
    uint addr, tmp;

    tmp = get_howmany_para(p);
    if(tmp != 1)
        goto error;
    p = str_to_hex(p, &addr);
cp:
    lprint("block %x ", addr);
    nand_reset();
    if(is_marked_bad_block(addr))
	lprint("is Marked bad.\r\n");
    else
    	lprint("is not marked bad!\r\n");
    return;

error:
    lprint("Error para!\r\nnander (hex block addr)\r\n");

}

void nandspr(unsigned char *p)
{
    uint addr, tmp;

    tmp = get_howmany_para(p);
    if(tmp != 1)
        goto error;
    p = str_to_hex(p, &addr);
cp:
    nand_reset();
    lprint("%x at nand spare addr %x\r\n", random_read_nand(1, addr), addr);
    return;

error:
    lprint("Error para!\r\nnandspr (hex addr) random read nand spare\r\n");

}

void nandr(unsigned char *p)
{
    uint addr, tmp;

    tmp = get_howmany_para(p);
    if(tmp != 1)
        goto error;
    p = str_to_hex(p, &addr);
cp:
    nand_reset();
    lprint("%x at nand addr %x\r\n", random_read_nand(0, addr), addr);
    return;

error:
    lprint("Error para!\r\nnandr (hex addr) random read nand\r\n");

}

void nander(unsigned char *p)
{
    uint addr, tmp;

    tmp = get_howmany_para(p);
    if(tmp != 1)
        goto error;
    p = str_to_hex(p, &addr);
cp:
    nand_reset();
    if(nand_erase_ll(addr))
	lprint("erase error\r\n");
    else
    	lprint("erase nand block 0x%x done!\r\n",addr);
    return;

error:
    lprint("Error para!\r\nnander (hex block addr)\r\n");

}

void nandcp(unsigned char *p)
{
    uint addr, pages, tmp;

    tmp = get_howmany_para(p);
    if(tmp != 2)
        goto error;
    p = str_to_hex(p, &addr);
    str_to_hex(p, &pages);
    addr = addr & 0xfffffe00;
cp:
    nand_reset();
    nand_read_ll(mrw_addr, addr, 512 * pages);
    lprint("cp 0x%x pages from nand addr %x to memory 0x%x done!\r\n",pages,addr,mrw_addr);
    return;

error:
    lprint("Error para!\r\nnandcp (hex addr) (hex pages)\r\n");

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
    lprint("Error para!\r\nw (hex value) [(hex addr)](last addr if no this argu)\r\n");

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
	lprint("\r\n\r\nHello, this is clean_boot v%sbuild on %s %s.\r\n", CLEAN_BOOT_VERSION,__DATE__,__TIME__);
	lmemset(cmd_buf, 0, COM_MAX_LEN);
	cmd_buf_p = 0;
	lprint("\r\nCleanBoot@%s>", PLATFORM);
	while(1){
		c = con_recv();
		if(c == ENTER_CHAR){
			handle_cmd();
			lmemset(cmd_buf, 0, COM_MAX_LEN);
			cmd_buf_p = 0;
			lprint("\r\nCleanBoot@%s>", PLATFORM);
		}else if(c == 0x1b){
			lmemset(cmd_buf, 0, COM_MAX_LEN);
			cmd_buf_p = 0;
			lprint("\r\nCleanBoot@%s>", PLATFORM);
		}else{
			if(cmd_buf_p < (COM_MAX_LEN - 1)){
				cmd_buf[cmd_buf_p++] = c;
				con_send(c);
			}
		}
	}
}