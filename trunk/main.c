#include <stdarg.h>
void print_help(void);
void print_time(void);
struct command{
     char * cmd_name;
     void (*cmd_fun)(void);
};

/*
const struct command cmd_list[]={
    {"time",print_time},
	{"help",print_help},
	{"ps",print_task},
	{"free",print_free},
	{"kill",kill_task},
	{"reboot",reboot},
    {"xm",x_m},
	{"dm",d_m},
	{"debug",dbg},
    {"w",write_mem},
    {"r",read_mem},
    {"sl811",sl811_main},
    {"sw",sl811_w},
    {"sr",sl811_r},
    {"upr",upr},
    {"ls",List},
};
*/
#define uch unsigned char
#define uint unsigned int
#define ULCON0 (*(volatile unsigned *)0x03ffd000)    //uart channel0 line control register
#define UCON0 (*(volatile unsigned *)0x03ffd004)    //uart channel0 control register
#define USTAT0 (*(volatile unsigned *)0x03ffd008)    //uart channel0 status register
#define UTXBUF0 (*(volatile unsigned *)0x03ffd00c)    //uart channel0 transmit holding register
#define URXBUF0 (*(volatile unsigned *)0x03ffd010)    //uart channel0 receive holding register
#define UBRDIV0 (*(volatile unsigned *)0x03ffd014)    //uart channel0 baud rate divisor register


void printk(char * fmt, ...);


void uart_init()
{
    ULCON0 = 0x03;
    UCON0 = 0x09;
    UBRDIV0 = 0x500;  //19200bps 50Mhz    
}
unsigned int send_ready()
{
    return (USTAT0&0x40);
}

unsigned int recv_ready()
{
    return (USTAT0&0x20);
}

void send_byte(unsigned char b)
{
   while(!send_ready());
   UTXBUF0 = b;
}
void send_string(const char*s)
{
   while(*s)
       send_byte(*(s++));
}

uch s_byte_to_asc(uch b)
{
	b&=0xf;
	b+=0x30;
	if(b>0x39)b+=7;
	return b;
}

uch* d_byte_to_str(uch b,uch*s)
{
	*(s+1)=s_byte_to_asc(b);
	b>>=4;
	*s=s_byte_to_asc(b);
	return s;
}

int is_son(char *s, char *dad)
{
    //printk("%s vs %s\r\n",s,dad);
    while(*s){
    //    send_byte(*s);
    //    send_byte(*dad);
        if(*s++ != *dad++)
            return 0;
    }
    return 1;    
}

uch s_asc_to_byte(uch tmp)
{

	if(tmp>='A' && tmp<='F')
	{
		return (tmp-0x37);
	}
	else if(tmp>='a' && tmp<='f')
	{
		return (tmp-'a'+0xa);
	}
	else if(tmp>='0' && tmp<='9')
	{
		return (tmp-0x30);
	}
	else
	{
		return 0;
	}
}

void send_asc_byte(uch n)
{
	uch ctmp[2];
	d_byte_to_str(n,ctmp);
	send_byte(ctmp[0]);
	send_byte(ctmp[1]);
}

void send_int(unsigned int it)
{
    unsigned int bv=1000000000;
	unsigned char s,k;

    for(k=0;k<10;k++){
	    s=it/bv;
        send_byte(s_byte_to_asc(s));
		it-=s*bv;
		bv/=10;
	}
}

void send_x_int(unsigned int it)
{
    unsigned char *cp = (unsigned char *)&it;

    cp += 3;
	send_asc_byte(*(cp--));
	send_asc_byte(*(cp--));
	send_asc_byte(*(cp--));
	send_asc_byte(*(cp));
}


void printk(char * fmt, ...)
{
    const char *s;
    int d;
    va_list ap;

    va_start(ap, fmt);
    while (*fmt) {
        if (*fmt != '%') {
			send_byte(*fmt++);
            continue;
        }
        switch (*++fmt) {
		    case '%':
			    send_byte(*fmt);
				break;
            case 's':
                s = va_arg(ap, char *);
                send_string(s);
                break;
            case 'd':
                d = va_arg(ap, int);
                send_int(d);
                break;
			case 'c':
                d = va_arg(ap, unsigned char);
                send_int(d);
                break;
			case 'x':
                d = va_arg(ap, int);
                send_x_int(d);
                break;
            /* Add other specifiers here... */             
            default: 
                send_byte(*(fmt-1));
				send_byte(*fmt);
                break;
        }
        fmt++;
    }
    va_end(ap);
    return; 
}

void print_mem(const char*p,unsigned int len)
{
    char *cp = p, tmp,k=0;

	while(len){
	    for(tmp=0;tmp<16;tmp++)
		{
		    send_asc_byte(*(cp++));
			send_string(" ");
			len--;
            k++;
			if(!len)break;
		}
		send_string("\r\n");
        if(!k)send_string("\r\n");
    }
	send_string("\r\n");
}

uch serial_received(uch *s)
{
    if(recv_ready()){
        *s = URXBUF0;
        return 1;
    }
    else
        return 0;
}

uint hex_asc_to_uint(char*p)
{
    uint tmp32=0;
    
    while(*p && *p != ' '){
        //send_byte (*p);
        tmp32 = (tmp32 << 4) + s_asc_to_byte(*p++);
    }
    return tmp32;
}


#define TOTAL_CMDS (sizeof(cmd_list)/sizeof(struct command))
#define VERSION "0.4"
#define MAX_COM_LEN 0x20
char command_buf[MAX_COM_LEN];
unsigned int com_buf_p = 0;

const char c_n[]="help";
char *cmd_para;
void reboot()
{
    void (*func)(void);
    func =0;
    (*func)();
}

uint lstrlen(uch *s)
{
    uint len=0;

    while(*s++)len++;
	return len;
}


uint mrw_addr=0;
void read_mem_32()
{
    uint value;
    char *p;

    p=cmd_para;
    //printk("p= %s\r\n",p);
	if((lstrlen(p)) == 0)goto read;

    while(*p == ' ')p++;
	//if((lstrlen(p))<8)goto error;
    mrw_addr = hex_asc_to_uint(p);
    send_x_int(mrw_addr);
    printk("addr is %x\r\n",mrw_addr);
read:
    value = *(uint*)mrw_addr;
    printk("Read 0x%x at xdata memory 0x%x\r\n",value,mrw_addr);

	return;
error:
	printk("Error para!\r\n");
    printk("r [(hex addr)](last addr if no this argu)\r\n");

}

void write_mem_32()
{
    uint value;
    char *p;

    p=cmd_para;
    //printk("p= %s\r\n",p);
    
    if(*p != ' ')goto error;
    while(*p == ' ')p++;
   	if((lstrlen(p)) == 0)goto error;
	//if((lstrlen(p))<8)goto error;
	printk("value = %s\r\n",p);
    value = hex_asc_to_uint(p);
    while(*p != ' ' && *p)p++;
    if((lstrlen(p)) == 0)goto write;
    if(*p != ' ')goto error;
    while(*p == ' ')p++;
   	if((lstrlen(p)) == 0)goto error;
   	printk("addr = %s\r\n",p);
   	mrw_addr = hex_asc_to_uint(p);
    //send_x_int(mrw_addr);
    //printk("addr is %x\r\n",mrw_addr);
write:
     *(uint*)mrw_addr = value;
    printk("write 0x%x at memory 0x%x\r\n",value,mrw_addr);

	return;
error:
	printk("Error para!\r\n");
    printk("w32 [(hex addr)](last addr if no this argu) [hex value]\r\n");

}

void write_mem_8()
{
    uint value;
    char *p;
  
    p=cmd_para;  
	printk("1 = %s\r\n",p);
    if(*p != ' ')goto error;
    while(*p == ' ')p++;
    printk("2 = %s\r\n",p);
   	if((lstrlen(p)) == 0)goto error;
	//if((lstrlen(p))<8)goto error;
	printk("value = %s\r\n",p);
    value = hex_asc_to_uint(p);
    while(*p != ' ' && *p)p++;
    if((lstrlen(p)) == 0)goto write;
    if(*p != ' ')goto error;
    while(*p == ' ')p++;
   	if((lstrlen(p)) == 0)goto error;
   	printk("addr = %s\r\n",p);
   	mrw_addr = hex_asc_to_uint(p);
    //send_x_int(mrw_addr);
    //printk("addr is %x\r\n",mrw_addr);
write:
     *(uch*)mrw_addr = (uch)value;
    printk("write 0x%x at memory 0x%x\r\n",value,mrw_addr);

	return;
error:
	printk("Error para!\r\n");
    printk("w32 [(hex addr)](last addr if no this argu) [hex value]\r\n");

}

void read_mem_8()
{
    uint value;
    char *p;

    p=cmd_para;
    //printk("p= %s\r\n",p);
	if((lstrlen(p)) == 0)goto read;

    while(*p == ' ')p++;
	//if((lstrlen(p))<8)goto error;
    mrw_addr = hex_asc_to_uint(p);
    //send_x_int(mrw_addr);
    //printk("addr is %x\r\n",mrw_addr);
read:
    value = *(uch*)mrw_addr;
    printk("Read 0x%x at xdata memory 0x%x\r\n",value,mrw_addr);

	return;
error:
	printk("Error para!\r\n");
    printk("r [(hex addr)](last addr if no this argu)\r\n");

}


void handle_cmd()
{
//    uint i,totalcmd =TOTAL_CMDS;
    char *p_cmd,*p_buf;
 //   struct command * p_cmdlist = cmd_list;
    
    printk("\r\n");
	p_buf=command_buf;
	if(!command_buf[0])return;
	//printk("0\r\n");
	if(is_son("help",p_buf) ){
	    cmd_para = p_buf + 4;
	    print_help();
	    return;
	}
	else if(is_son("reboot",p_buf) ){
	    reboot();
	    return;
	}
	else if(is_son("r32",p_buf) ){
	    cmd_para = p_buf + 3;
	    read_mem_32();
	    return;
	}
	else if(is_son("r8",p_buf) ){
	    cmd_para = p_buf + 2;
	    read_mem_8();
	    return;
	}
	else if(is_son("w32",p_buf) ){
	    cmd_para = p_buf + 3;
	    write_mem_32();
	    return;
	}
	else if(is_son("w8",p_buf) ){
	    cmd_para = p_buf + 2;
	    write_mem_8();
	    return;
	}

//if(!lstrcmp(command_buf,"time"))printk("The count is ");
    printk("Unknow command:%s\r\n",command_buf);
}

void print_help()
{
	printk("help\r\n");
	printk("reboot\r\n");
	printk("r32\r\n");
}



void lmemset(char *d,char v,unsigned int n)
{
	while(n--)*d++=v;
}


int Main()
{
    uch tmp;
    uart_init();
	printk("\r\n\r\nHello, this is CleanOS@51 V%s\r\n",VERSION);
    printk("kjkda%s, %d\r\n","ddddd",(uint)78);
    print_mem((char *)0x8000,0x200);
    printk("CleanOS@51>");
/*    while(1){
        while(!serial_received(&tmp));
        if(tmp != 0x0d){
            send_byte(tmp);
		}
    }    */
    com_buf_p = 0;
    lmemset(command_buf,0,MAX_COM_LEN);
    while(1)
    {
		while(!serial_received(&tmp));
		//send_asc_byte(tmp);
		if(tmp != 0x0d){
		    if(com_buf_p<MAX_COM_LEN-2){
			    command_buf[com_buf_p++]=tmp;
                send_byte(tmp);
			}
		}
		else{

            //command_buf[com_buf_p]=0;
            //printk("%s\n",command_buf);
            handle_cmd();
			lmemset(command_buf,0,MAX_COM_LEN);
            printk("CleanOS@51>");
			com_buf_p=0;
		}    
		//printk("Received Byte 0x%x\r\n",tmp);
    }

    return 0;
}