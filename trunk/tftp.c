#include "cs8900.h"
#include "type.h"
#include "tcp.h"
#include "interface.h"

#define code
code const char tftp_req_exam2[]=
{
0x00,0xe0,0xb0,0xea,0x14,0xf7,0x08,0x00,  0x3e,0x26,0x0a,0x5b,0x08,0x00,0x45,0x00,
0x00,0x3d,0x07,0xc4,0x40,0x00,0xff,0x11,  0x7e,0x3c,0xc0,0xa8,0x3a,0x33,0xc0,0xa8,
0x3a,0x2b,0x05,0xfd,0x00,0x45,0x00,0x29,  0x00,0x00,0x00,0x01,0x73,0x00,0x6f,0x63,
0x74,0x65,0x74,0x00,0x74,0x69,0x6d,0x65,  0x6f,0x75,0x74,0x00,0x35,0x00,0x62,0x6c,
0x6b,0x73,0x69,0x7a,0x65,0x00,0x31,0x34,  0x36,0x38,0x00,
};
code const char tftp_req_exam[]=
{
0x00,0xe0,0xb0,0xea,0x14,0xf7,0x00,0x0c,  0x29,0x50,0xf2,0x0d,0x08,0x00,0x45,0x00,
0x00,0x32,0x00,0x00,0x40,0x00,0x40,0x11,  0x45,0x12,0xc0,0xa8,0x3a,0x2d,0xc0,0xa8,
0x3a,0x2b,0x80,0x61,0x00,0x45,0x00,0x1e,  0x46,0x60,0x00,0x01,0x63,0x6c,0x65,0x61,
0x6e,0x62,0x6f,0x6f,0x74,0x2e,0x62,0x69,  0x6e,0x00,0x6f,0x63,0x74,0x65,0x74,0x00,
};

code const char tftp_req[]=
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x08,0x00,0x45,0x00,
0x00,0x32,0x00,0x00,0x40,0x00,0x40,0x11,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x70,0x23,0x00,0x45,0x00,0x1e,  0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

code const long arp_req[]=
{0xffffffff,0x4408ffff,0x090a0023,0x01000608,
 0x08000604,0x00010844,0x090a0023,0xc0a83a23,
 0x00000000,0x0000c0a8,0x3a230000,0x00000000,
 0x00000000,0x00000000,0x00000000};

code const char arp_resp[]=
{
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x06,0x00,0x01,
0x08,0x00,0x06,0x04,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

code const long tftp_ack[]=
{0x00508d6e,0x31270844,0x090a0023,0x08004500,
 0x0020006c,0x00008011,0x44cfc0a8,0x3a23c0a8,
 0x3a1e0416,0x04e8000c,0x013f0004,0x00010000,
 0x00000000,0x00000000,0x00000000};

#define MAX_PACKAGE 1518
static struct tftp_status
{
	uint filesize;
	unsigned short port;
	unsigned short block_n;
	unsigned char *filename;
	unsigned char *membase;
	unsigned char running;
	unsigned char operation;
	unsigned char next_stop;
} t_s;
static unsigned char rsp_arp_buf[60];
static unsigned char s_buf[MAX_PACKAGE];
static unsigned char r_buf[MAX_PACKAGE];
extern const unsigned char * cs8900_mac[];
static unsigned char server_mac[6];
extern uint server_ip;
extern uint local_ip;
static struct eth_h*sep = (struct eth_h*)s_buf, *rep = (struct eth_h*)r_buf;
static struct ip_udp_tftp* siutp;
static struct ip_udp_tftp* riutp;
static uint send_len;
static unsigned short ipid = 1;

unsigned short for_check(unsigned char *crcdata,uint len)
{
        unsigned short *t,sum=0,new_sum;
        uint jw=0,i;

        t = (unsigned short *)crcdata;
    	for(i=0;i<len/2;i++){
                new_sum = *t + sum;
                if(new_sum<sum)jw++;
                sum=new_sum;
                t++;
        }
    	sum+=jw;
        sum=~sum;
        //*(unsigned int xdata*)crccode = sum;
        return sum;
}

uint lstrlen(unsigned char*s)
{
	uint l = 0;
	while(*s++)
		l++;
	return l;
}

uint lmemequ(unsigned char *a, unsigned char * b, uint len)
{
	while(len--)
		if(*a++ != *b++)
			return 0;
	return 1;
}

void lstrcpy(unsigned char *d, unsigned char*s)
{
	while(*d++ = *s++);
}

unsigned short change_end(unsigned short a)
{
	return ((0xff & a)<<8) + ((0xff00 & a)>>8);
}	

void tftp_put(unsigned char* name, uint sz, unsigned char *buf)
{
	/*init t_s*/
	t_s.block_n = 0;
	t_s.filename = name;
	t_s.membase = buf;
	t_s.operation = 0;//put file to server
	t_s.filesize = sz;
	t_s.next_stop = 0;
	tftp_run();
}
	
void tftp_get(unsigned char* name, unsigned char *buf)
{
	/*init t_s*/
	t_s.block_n = 1;
	t_s.filename = name;
	t_s.membase = buf;
	t_s.operation = 1;//get file from server
	tftp_run();
}

static void setup_arp_req(unsigned char * buf)
{
	struct eth_h* tmp_ep = (struct eth_h*)buf;
	struct arp_p *arp_p_p = (struct arp_p *)tmp_ep->datas;
	/*clear buffer*/
	lmemset(buf, 0, 60);
	/*set 802.3 header*/
	lmemset(tmp_ep->dest_mac, 0xff, 6);
	lmemcpy(tmp_ep->src_mac, cs8900_mac, 6);
	tmp_ep->protocol = 0x0608;
	/*setup arp package*/
	arp_p_p->hardware = 0x100;
	arp_p_p->protocol = 0x08;
    	arp_p_p->hardware_addr_len = 6;
    	arp_p_p->protocol_addr_len = 4;
    	arp_p_p->operation = 0x100;
	lmemcpy(arp_p_p->sender_mac, cs8900_mac, 6);
	lmemset(arp_p_p->target_mac, 0x0, 6);
    	lmemcpy(arp_p_p->sender_ip, &local_ip, 4);
    	lmemcpy(arp_p_p->target_ip, &server_ip, 4);
	send_len = 60;
}
	
uint anlz_arq()
{
	struct arp_p *arp_p_p = (struct arp_p *)rep->datas;
	if(rep->protocol != 0x0608 || arp_p_p->operation != 0x0200 \
				|| !lmemequ(arp_p_p->sender_ip, (unsigned char*)&server_ip, 4))
		return 0;//failed
	lmemcpy(server_mac, arp_p_p->sender_mac, 6);
	return 1;
}

uint recv_p()
{
	struct eth_h*tmp_ep = (struct eth_h*)rsp_arp_buf;
	struct arp_p *arp_p_p = (struct arp_p *)rep->datas, *rsp_arp = (struct arp_p *)tmp_ep->datas;
	uint len;

	len = cs8900_recv(r_buf);
	if(!len)
		return 0;
	if(rep->protocol == 0x0608 && arp_p_p->operation == 0x0100){
		if( lmemequ(arp_p_p->target_ip, &local_ip, 4)
		    && lmemequ(arp_p_p->sender_ip, &server_ip, 4)){
			setup_arp_req(rsp_arp_buf);
			lmemcpy(tmp_ep->dest_mac, rep->src_mac, 6);
    			rsp_arp->operation = 0x200;
			lmemcpy(rsp_arp->target_mac, arp_p_p->sender_mac, 6);
    			lmemcpy(rsp_arp->target_ip, arp_p_p->sender_ip, 4);
			if(cs8900_send(rsp_arp_buf, 60) != 60)
				lprint("answer asp failed!\r\n");
			else
				lprint("recv asp and answered\r\n");
			while(1);
		}
		return 0;
	}
	return len;
}
	
uint get_response(uint (*anlz)(), uint try)
{
	uint len, wait;
	
send:
        len = cs8900_send(s_buf, send_len);
        if(len != send_len){
                lprint("send packages error\r\n");
                return 0;
        }
try_recv:
        wait = 300;
        while(!(len = recv_p())){
                delay_us(10000);
                if(!wait--)
                        break;
        }
        if(!len){
		if(try--){
                	lprint("no response, retrying\r\n");
        	        delay_us(1000000);
       		        goto send;
		}
		else{
                	lprint("stop trying, give up.\r\n");
			return 0;
		}
        }
        if(!(*anlz)())
                goto try_recv;
	return 1;
}	

void setup_tftp_req()
{
	unsigned char * s;
	unsigned short local_port, udp_len;

	/*setup tftp req package*/
	lmemset(s_buf, 0, MAX_PACKAGE);
	lmemcpy(s_buf, tftp_req, 64);
	/*set 802.3 header*/
	lmemcpy(sep->dest_mac, server_mac, 6);
	lmemcpy(sep->src_mac, cs8900_mac, 6);

    	siutp->udp_header.check_sum = 0;
    	local_port = random_u16();
    	while(local_port < 2000)
		local_port *= 3;
	t_s.port = local_port;
	siutp->udp_header.src_port = change_end(local_port);
    	udp_len = 17 + lstrlen(t_s.filename);
    	siutp->udp_header.length = change_end(udp_len);
	/*setup ip header*/
    	siutp->ip_header.id = change_end(ipid++);
    	siutp->ip_header.total_len = change_end(20+udp_len);
    	lmemcpy(siutp->ip_header.sender_ip, &local_ip, 4);
    	lmemcpy(siutp->ip_header.target_ip, &server_ip, 4);
	siutp->ip_header.check_sum = 0;
	siutp->ip_header.check_sum = for_check(&siutp->ip_header.ipv_hdl, 20);
    	siutp->tftp_packet.operation = t_s.operation ? change_end(1):change_end(2);
	/*setup filename */
    	s = (char*)&siutp->tftp_packet.block_n;
    	s = (char*)&siutp->tftp_packet.block_n;
    	lstrcpy(s, t_s.filename);
	s += lstrlen(s) + 1;
    	lstrcpy(s, "octet");
	send_len = 51 + lstrlen(t_s.filename);
	if(send_len < 60)
		send_len = 60;
}
	
uint anlz_tftp()
{
	unsigned short data_len;
	uint tmp;
	if(rep->protocol != 0x0008 || riutp->ip_header.protocol != 0x11 
				   || riutp->udp_header.dest_port != change_end(t_s.port))
		return 0;
	if(riutp->tftp_packet.operation == 0x500){
		t_s.running = 0;
		lprint("Error message from server: code %x, '%s'\r\n", change_end(riutp->tftp_packet.block_n),riutp->tftp_packet.tftp_data); 
		return 1;
	}
	if(t_s.operation){
		//tftp get case
		if(riutp->tftp_packet.operation != 0x300 
			|| change_end(riutp->tftp_packet.block_n) != t_s.block_n){
			lprint("Unusual error!\r\n");
			return 0;
		}
		data_len = change_end(riutp->udp_header.length) - 12;		
		con_send('.');
		if(data_len == 512)
			t_s.running = 1;	
		else{
			t_s.running = 0;
			lprint("\r\nfile size:0x%x(%d)\r\n", t_s.filesize = (t_s.block_n-1)*512 + data_len, t_s.filesize);
		}
		lmemcpy(t_s.membase, riutp->tftp_packet.tftp_data, data_len);
		t_s.membase += 512;
		//setup ack package
		/*setup tftp req package*/
		lmemset(s_buf, 0, MAX_PACKAGE);
		lmemcpy(s_buf, tftp_req, 64);
		/*set 802.3 header*/
		lmemcpy(sep->dest_mac, server_mac, 6);
		lmemcpy(sep->src_mac, cs8900_mac, 6);

    		siutp->udp_header.check_sum = 0;
		siutp->udp_header.src_port = change_end(t_s.port);
		/*setup ip header*/
    		lmemcpy(siutp->ip_header.sender_ip, &local_ip, 4);
    		lmemcpy(siutp->ip_header.target_ip, &server_ip, 4);
		siutp->ip_header.total_len = change_end(32);
		siutp->ip_header.id = change_end(ipid++);
		siutp->ip_header.check_sum = 0;
		siutp->ip_header.check_sum = for_check(&siutp->ip_header.ipv_hdl, 20);
		/*udp & tftp*/
		siutp->udp_header.dest_port = riutp->udp_header.src_port;
		siutp->udp_header.length = change_end(12);
		siutp->tftp_packet.operation = 0x400;
		siutp->tftp_packet.block_n = change_end(t_s.block_n);

		send_len = 60;
		t_s.block_n++;
		if(!t_s.running)
        		cs8900_send(s_buf, send_len);
		return 1;
	}
	else{
		//tftp put case
		if(riutp->tftp_packet.operation != 0x400 
			|| change_end(riutp->tftp_packet.block_n) != t_s.block_n){
			lprint("Unusual error!\r\n");
			return 0;
		}
		t_s.running =1;
		//setup ack package
		/*setup tftp req package*/
		lmemset(s_buf, 0, MAX_PACKAGE);
		lmemcpy(s_buf, tftp_req, 64);
		data_len = (t_s.filesize > 512)?512:t_s.filesize;		
		con_send('`');
		if(t_s.next_stop){
			t_s.running = 0;
			lprint("\r\n");
			return 1;
		}
		if(data_len != 512){
			t_s.next_stop = 1;
		}
		lmemcpy(siutp->tftp_packet.tftp_data, t_s.membase, data_len);
		t_s.membase += data_len;
		t_s.filesize -= data_len;
		/*set 802.3 header*/
		lmemcpy(sep->dest_mac, server_mac, 6);
		lmemcpy(sep->src_mac, cs8900_mac, 6);

    		siutp->udp_header.check_sum = 0;
		siutp->udp_header.src_port = change_end(t_s.port);
		/*setup ip header*/
    		lmemcpy(siutp->ip_header.sender_ip, &local_ip, 4);
    		lmemcpy(siutp->ip_header.target_ip, &server_ip, 4);
		siutp->ip_header.total_len = change_end(32 + data_len);
		siutp->ip_header.id = change_end(ipid++);
		siutp->ip_header.check_sum = 0;
		siutp->ip_header.check_sum = for_check(&siutp->ip_header.ipv_hdl, 20);
		/*udp & tftp*/
		siutp->udp_header.dest_port = riutp->udp_header.src_port;
		siutp->udp_header.length = change_end(data_len + 12);		
		siutp->tftp_packet.operation = 0x300;
		siutp->tftp_packet.block_n = change_end(++t_s.block_n);

		send_len = 46 + data_len;
		if(send_len<60)
			send_len = 60;
		return 1;
		
	}
	return 0;
}
	
static void tftp_run()
{
        sep = (struct eth_h*)s_buf; 
	rep = (struct eth_h*)r_buf;
        siutp = (struct ip_udp_tftp*) sep->datas;
        riutp = (struct ip_udp_tftp*) rep->datas;

	if(!cs8900_is_ready()){
		lprint("cs8900 not ready!\r\n");
		return;
	}
	cs8900_open();
	setup_arp_req(s_buf);	
	if(!get_response(anlz_arq, 3)){
		lprint("server no response!\r\n");
		return;
	}
	setup_tftp_req();
	if(!get_response(anlz_tftp,0)){
		lprint("server no tftp response!\r\n");
		return;
	}
	while(t_s.running){
		if(!get_response(anlz_tftp, 0)){
			lprint("lost connection with server while transfering!\r\n");
			return;
		}
	}
	cs8900_halt();
	lprint("tftp operation finished successfully!\r\n");
	return;
}
#if 0
void tftp_put(unsigned char* name)
{
	struct eth_h*ep = p_buf;
	struct arp_p *arp_p_p = (struct arp_p *)ep->datas;
	struct ip_udp_tftp* iutp = (struct ip_udp_tftp*) ep->datas;
	uint len, wait, len1;
	char *s;

lprint("name is %s\n", name);
	if(!cs8900_is_ready()){
		lprint("cs8900 not ready!\r\n");
		return;
	}
	cs8900_open();
	/*clear buffer*/
	lmemset(p_buf, 0, MAX_PACKAGE);
	/*set 802.3 header*/
	lmemset(ep->dest_mac, 0xff, 6);
	lmemcpy(ep->src_mac, cs8900_mac, 6);
	ep->protocol = 0x0608;
	/*setup arp package*/
	arp_p_p->hardware = 0x100;
	arp_p_p->protocol = 0x08;
    	arp_p_p->hardware_addr_len = 6;
    	arp_p_p->protocol_addr_len = 4;
    	arp_p_p->operation = 0x100;
	lmemcpy(arp_p_p->sender_mac, cs8900_mac, 6);
	lmemset(arp_p_p->target_mac, 0x0, 6);
    	lmemcpy(arp_p_p->sender_ip, &local_ip, 4);
    	lmemcpy(arp_p_p->target_ip, &server_ip, 4);
	/*send package*/
send_arp:
        len = cs8900_send(p_buf, 60);
	if(len != 60){
        	lprint("send packages error\r\n");
		return;
	}
	wait = 10;
recv_package:
	while(!(len = cs8900_recv(p_buf))){
		delay_us(1000);
		if(!wait--)
			break;
	}
	if(!len){
		lprint("get arp response failed, retrying\r\n");
		delay_us(1000000);
		goto send_arp;
	}
	if(ep->protocol != 0x0608 || arp_p_p->operation != 0x0200)
		goto recv_package;
	lmemcpy(server_mac, arp_p_p->sender_mac, 6);
	print_mem(server_mac, 6);

	/*start send tftp request*/
	/*setup tftp req package*/
	lmemset(p_buf, 0, MAX_PACKAGE);
	//lmemcpy(p_buf, tftp_req_exam, 64);

	lmemcpy(p_buf, tftp_req, 64);
	/*set 802.3 header*/
	lmemcpy(ep->dest_mac, server_mac, 6);
	lmemcpy(ep->src_mac, cs8900_mac, 6);

    	iutp->ip_header.id = 0x01;
    	lmemcpy(iutp->ip_header.sender_ip, &local_ip, 4);
    	lmemcpy(iutp->ip_header.target_ip, &server_ip, 4);
	print_mem(p_buf, 60);
lprint("orgin:\r\n");
print_mem(iutp->ip_header.ipv_hdl, 20);
	lprint("check sum is %x bf\r\n", iutp->ip_header.check_sum);
	iutp->ip_header.check_sum = 0;
	iutp->ip_header.check_sum = for_check(&iutp->ip_header.ipv_hdl, 20);
	lprint("check sum is %x after\r\n", iutp->ip_header.check_sum);
	lprint("check %x after sum\r\n", for_check(&iutp->ip_header.ipv_hdl, 20));
    	iutp->udp_header.check_sum = 0;
    	iutp->udp_header.src_port = random_u16();
    	while(iutp->udp_header.src_port < 2000)
		iutp->udp_header.src_port *= 3;
    	iutp->udp_header.length = change_end(17 + lstrlen(name));
    	s = (char*)&iutp->tftp_packet.block_n;
print_mem(s, 32);
    	lstrcpy(s, name);
print_mem(s, 32);
	s += lstrlen(s) + 1;
print_mem(s, 32);
    	lstrcpy(s, "octet");
print_mem(s, 32);
	len = 50 + lstrlen(name);
	if(len < 60)
		len = 60;
	len = 64;
	lprint("send len is %x \r\n", len);
send_tftp_req:
        len1 = cs8900_send(p_buf, len);
	if(len != len){
        	lprint("send packages error\r\n");
		return;
	}
	wait = 10;
	
	cs8900_halt();
	lprint("cs8900 is ready, please wait code ready!\r\n");
}
#endif

