/*
 *  asm2440.S - Startup Code for S3c2440 CPU-core
 *
 * Modified from u-boot start.S
 * By Clean Li 2009.4 China
 */

.globl send_string
.globl serial_send_byte
.globl main
.globl _start
_start:	b       start_code
	b	_start
	b	_start
	b	_start
	b	_start
	b	_start
	b	_start
	b	_start
	b	_start
	.balignl 16,0xdeadbeef
.align 4
filldata:
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
	.long 0
/*
 * the actual start code
 */

start_code:
	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0,cpsr
	bic	r0,r0,#0x1f
	orr	r0,r0,#0xd3
	msr	cpsr,r0


#  define pWTCON		0x53000000
#  define INTMSK		0x4A000008	/* Interupt-Controller base addresses */
#  define INTSUBMSK	0x4A00001C
#  define CLKDIVN	0x4C000014	/* clock divisor register */

	ldr     r0, =pWTCON
	mov     r1, #0x0
	str     r1, [r0]

/*init sp*/
	ldr	sp, =0x00000ff8

	/*
	 * mask all IRQs by setting all bits in the INTMR - default
	 */
	mov	r1, #0xffffffff
	ldr	r0, =INTMSK
	str	r1, [r0]

	ldr	r1, =0x3ff
	ldr	r0, =INTSUBMSK
	str	r1, [r0]

	/* FCLK:HCLK:PCLK = 1:2:4 */
	/* default FCLK is 120 MHz ! */
	ldr	r0, =CLKDIVN
	mov	r1, #3
	str	r1, [r0]

/*my loop*/
#  define GPFCON		0x56000050
#  define GPFDAT		0x56000054	/*  */

	ldr	r0, =GPFCON
	mov	r1, #0x5500
	str	r1, [r0]
#define GPHCON 	0x56000070
#define GPHUP	0x56000078
#define ULCON0 	0x50000000
#define UCCON0 	0x50000004
#define UFCON0 	0x50000008
#define USCON0 	0x50000010
#define UTXH0 	0x50000020
#define URXH0 	0x50000024
#define UBRDIV0 0x50000028

/*Init serial port 0*/
	ldr	r0, =GPHCON
	ldr	r1, =0xaaa0
	str	r1, [r0]
	ldr	r0, =GPHUP
	ldr	r1, =0x7ff
	str	r1, [r0]
	ldr	r0, =ULCON0
	mov	r1, #0x3
	str	r1, [r0]
	ldr	r0, =UCCON0
	mov	r1, #0x5
	str	r1, [r0]
	ldr	r0, =UBRDIV0
	mov	r1, #18
	str	r1, [r0]	/*set baudrate 9600bps*/

	b	main	

