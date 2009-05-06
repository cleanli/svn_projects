#include <stdarg.h>
#include "debug.h"
#include "print.h"

//#define div(X,Y) (X)/(Y)
void print_string(unsigned char *s)
{
        unsigned char c;
        while(1){
                if(!(c = *s++))
                        break;
                con_send(c);
        }
        return;

}

unsigned char halfbyte2char(unsigned char c)
{
        return ((c & 0x0f) < 0x0a)?(0x30 + c):('A' + c - 0x0a);
}

uint div(uint beichushu, uint chushu)
{
    uint shang = 0, tmp = 0;
    const unsigned char bit_of_uint = 8 * sizeof(uint);
    unsigned char n = bit_of_uint;

    while(n--){
        tmp = (tmp<<1) + (beichushu>>(bit_of_uint - 1));
        beichushu<<=1;
        shang = shang<<1;
        if(tmp >= chushu){
                tmp = tmp - chushu;
                shang++;
        }
    }
    return shang;
}


unsigned char * num2str(uint jt, unsigned char * s, unsigned char n)
{
        unsigned char * st, k = 1, j;
        uint tmp;

        st = s;
        if(n > 16 || n < 2){
                *st++ = 0x30;
                *st = 0;
                return s;
        }
        tmp = 1;
        while(div(jt, tmp) >= n){
                k++;
                tmp *= n;
        }

        while(k--){
                j = div(jt, tmp);
                *st++ = halfbyte2char(j);
                jt -= tmp * j;
                tmp = div(tmp, n);
        }
        *st = 0;
        return s;
}

void print_uint(uint num)
{
        unsigned char nc[11];
        num2str(num, nc, 10);
        print_string(nc);
}

void print_hex(uint num)
{
        unsigned char nc[9];
        num2str(num, nc, 16);
        print_string(nc);
}

void lprint(const unsigned char * fmt, ...)
{
    const unsigned char *s;
    uint d;
    va_list ap;

    va_start(ap, fmt);
    while (*fmt) {
        if (*fmt != '%') {
	    con_send(*fmt++);
            continue;
        }
        switch (*++fmt) {
	    case '%':
	        con_send(*fmt);
		break;
            case 's':
                s = va_arg(ap, const unsigned char *);
                print_string(s);
                break;
            case 'd':
                d = va_arg(ap, int);
                print_uint(d);
                break;
	    /*
	    case 'c':
                d = va_arg(ap, char);
                send_int(d);
                break;
	    */
	    case 'x':
                d = va_arg(ap, uint);
                print_hex(d);
                break;
            /* Add other specifiers here... */             
            default: 
                con_send(*(fmt-1));
		con_send(*fmt);
                break;
        }
        fmt++;
    }
    va_end(ap);
    return;   
}
