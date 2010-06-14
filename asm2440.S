/*
 *  asm2440.S - Startup Code for S3c2440 CPU-core
 *
 * Modified from u-boot start.S
 * By Clean Li 2009.4 China
 */

.globl send_string
.globl s3c2440_serial_send_byte
.globl main
.globl _start
_start:	b       start_code
        ldr     pc, =0x30000004
        ldr     pc, =0x30000008
        ldr     pc, =0x3000000c
        ldr     pc, =0x30000010
        ldr     pc, =0x30000014
        ldr     pc, =0x30000018
        ldr     pc, =0x3000001c
.align 4

mem_reg_value:
        .long   0x22111110
        .long   0x00000700
        .long   0x00000700
        .long   0x00000700
        .long   0x00000700
        .long   0x00000700
        .long   0x00000700
        .long   0x00018009
        .long   0x00018009
        .long   0x00ac03f4
        .long   0x000000b2
        .long   0x00000030
        .long   0x00000030
/*
 * the actual start code
 */

start_code:
	/*
	 * set the cpu to SVC32 mode
	 */
	msr	cpsr,#0xd3


#  define pWTCON		0x53000000
#  define INTMSK		0x4A000008	/* Interupt-Controller base addresses */
#  define INTSUBMSK	0x4A00001C
#  define CLKDIVN	0x4C000014	/* clock divisor register */
/*diable wtd*/
	ldr     r0, =pWTCON
	mov     r1, #0x0
	str     r1, [r0]

/*init sp*/
	@ldr	sp, =0x32ef0000
	ldr	sp, =0x00000ffc

	/*
	 * mask all IRQs by setting all bits in the INTMR - default
	 */
/*
	mov	r1, #0xffffffff
	ldr	r0, =INTMSK
	str	r1, [r0]

	ldr	r1, =0x3ff
	ldr	r0, =INTSUBMSK
	str	r1, [r0]
*/

/*******************************************************************************/
#  define GPFCON                0x56000050
#  define GPFDAT                0x56000054      /*  */
#  define LOCKTIME              0x4C000000
#  define MPLLCON               0x4C000004
#  define UPLLCON               0x4C000008
/*
        ldr     r0, =GPFCON
        mov     r1, #0x5500
        str     r1, [r0]
*/

        @initialize clock
        ldr     r0, =LOCKTIME
        ldr     r1, =0x00ffffff
        str     r1, [r0]
        /* FCLK:HCLK:PCLK = 1:4:8 */
        ldr     r0, =CLKDIVN
        mov     r1, #5
        str     r1, [r0]
        @open code cache
        mrc     p15,0,r1,c1,c0,0
        orr     r1,r1,#0xc0000000
        mcr     p15,0,r1,c1,c0,0
        @UPLL setup
        ldr     r0, =UPLLCON
        ldr     r1, =(0x38<<12|0x2<<4|0x2)
        str     r1, [r0]
        nop
        nop
        nop
        nop
        nop
        nop

        @MPLL setup
        ldr     r0, =MPLLCON
        ldr     r1, =0x7f021
        str     r1, [r0]
        nop
        nop
        nop
        nop
        nop
        nop

        @memsetup
        ldr     r0, =0x48000000
        adrl    r1, mem_reg_value
        ldr     r2, =0x48000030
loop10:
        ldr     r4, [r1], #4
        str     r4, [r0], #4
        cmp     r2, r0
        bne     loop10

        ldr     r0, =GPFDAT
        mov     r1, #0x50
        str     r1, [r0]

#define GPHCON  0x56000070
#define GPHUP   0x56000078
#define ULCON0  0x50000000
#define UCCON0  0x50000004
#define UFCON0  0x50000008
#define USCON0  0x50000010
#define UTXH0   0x50000020
#define URXH0   0x50000024
#define UBRDIV0 0x50000028

/*Init serial port 0*/
        ldr     r0, =GPHCON
        ldr     r1, =0xaaa0
        str     r1, [r0]
        ldr     r0, =GPHUP
        ldr     r1, =0x7ff
        str     r1, [r0]
        ldr     r0, =ULCON0
        mov     r1, #0x3
        str     r1, [r0]
        ldr     r0, =UCCON0
        mov     r1, #0x5
        str     r1, [r0]
        ldr     r0, =UBRDIV0
        mov     r1, #0x1b
        str     r1, [r0]        /*set baudrate 115200bps*/
@mov	r0, #'A'
@bl	s3c2440_serial_send_byte

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ copy code to ram from nand
@
#define NAND_CTL_BASE   0x4e000000
#define oNFCONF         0x0
#define oNFCONT         0x4
#define oNFCMD          0x8
#define oNFSTAT         0x20

        @ reset NAND
        mov     r1, #NAND_CTL_BASE
        ldr     r2, =( (7<<12)|(7<<8)|(7<<4)|(0<<0) )
        str     r2, [r1, #oNFCONF]

        ldr     r2, =( (1<<4)|(0<<1)|(1<<0) ) @ Active low CE Control
        str     r2, [r1, #oNFCONT]
        ldr     r2, [r1, #oNFCONT]

@        ldr     r2, =(0x6)              @ RnB Clear
@        str     r2, [r1, #oNFSTAT]
@        ldr     r2, [r1, #oNFSTAT]

        mov     r2, #0xff               @ RESET command
        strb    r2, [r1, #oNFCMD]
        mov     r3, #0                  @ wait
1:      add     r3, r3, #0x1
        cmp     r3, #0x2a
        blt     1b

@        ldr     r2, [r1, #oNFCONT]
@        orr     r2, r2, #0x2            @ Flash Memory Chip Disable
@        str     r2, [r1, #oNFCONT]
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        @ get ready to call C functions (for nand_read())
        @ldr     sp, DW_STACK_START      @ has been setuped at start

#define TEXT_RAM_BASE   0x33ffc000
#define TEXT_NAND_BASE  0x1000
#define TEXT_SIZE       0x1000

        @ copy minicb ENV block to ram base
        ldr     r0, =TEXT_RAM_BASE
        ldr     r1, =TEXT_NAND_BASE
        ldr     r2, =TEXT_SIZE
        bl      nand_read_ll
	@ find the active para, the first 0xffffffff
        ldr     r4, =TEXT_RAM_BASE
        ldr     r5, =TEXT_RAM_BASE + TEXT_SIZE
        ldr     r6, =0xffffffff
loop101:
        ldr     r7, [r5, #-4]!
        cmp     r7, r6
        bne     found_it
	cmp	r4, r5
	beq	main	@if not found go strait to main
	b	loop101
/*the para integer in nand should be : Rambase, Nandbase, Copysize, Magic*/	
found_it:
	cmp	r7, #0x000000cb
	bne	main
	ldr	r2, [r5, #-4]!	@before it will be the Copy Size
	ldr	r1, [r5, #-4]!	@before it is nand base
	ldr	r0, [r5, #-4]!	@before it is ram base
	stmfd	sp!, {r0-r2}
	mov	r3, r2
	mov	r2, r1
	mov	r1, r0
	ldr	r0, =print_para
	bl	lprint

	ldr	r0, =press_key
	bl	lprint

	mov	r0, #0x120000	@delay time
        ldr     r1, =TEXT_RAM_BASE + TEXT_SIZE	@no use value addr
	bl	time_limit_recv_byte
	cmp	r0, #0		@if key pressed, 
	beq	main		@go to main

	ldmfd	sp!, {r0-r2}
	mov	r5, r0		@store ram base 
        bl      nand_read_ll	@copy real program
	mov	pc, r5		@run the program
press_key:
	.string "Any key stop autoboot...\n"
print_para:
	.string "\nram=%x nand=%x size=%x\n"
                                           
/*****************************************************************************/
