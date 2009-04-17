#ifndef _INTERFACE_H
#define _INTERFACE_H

#include "s3c2440.h"

#define PLATFORM "S3C2440"

#define con_send(X) s3c2440_serial_send_byte(X)
#define con_recv() s3c2440_serial_recv_byte()
#define is_con_recv() s3c2440_is_serial_recv()
/*
#include "stdio.h"
#define con_send(X) putchar(X)
#define con_recv() getchar()
//#define is_con_recv() s3c2440_is_serial_recv()
*/

#endif
