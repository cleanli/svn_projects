/*
 *  asm2440.S - Startup Code for S3c2440 CPU-core
 *
 * Modified from u-boot start.S
 * By Clean Li 2009.4 China
 */

.globl nand_read_ll
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
	@ldr	sp, =0x00000ff8
	ldr	sp, =0x30101000

	/*
	 * mask all IRQs by setting all bits in the INTMR - default
	 */
	mov	r1, #0xffffffff
	ldr	r0, =INTMSK
	str	r1, [r0]

	ldr	r1, =0x7ff
	ldr	r0, =INTSUBMSK
	str	r1, [r0]

/*******************************************************************************/
#  define GPFCON                0x56000050
#  define GPFDAT                0x56000054      /*  */
#  define LOCKTIME              0x4C000000
#  define MPLLCON               0x4C000004
#  define UPLLCON               0x4C000008

        ldr     r0, =GPFCON
        mov     r1, #0x5500
        str     r1, [r0]

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
        ldr     r2, [r1, #oNFCONF]

        ldr     r2, =( (1<<4)|(0<<1)|(1<<0) ) @ Active low CE Control
        str     r2, [r1, #oNFCONT]
        ldr     r2, [r1, #oNFCONT]

        ldr     r2, =(0x6)              @ RnB Clear
        str     r2, [r1, #oNFSTAT]
        ldr     r2, [r1, #oNFSTAT]

        mov     r2, #0xff               @ RESET command
        strb    r2, [r1, #oNFCMD]
        mov     r3, #0                  @ wait
1:      add     r3, r3, #0x1
        cmp     r3, #0xa
        blt     1b
2:      ldr     r2, [r1, #oNFSTAT]      @ wait ready
        tst     r2, #0x4
        beq     2b

        ldr     r2, [r1, #oNFCONT]
        orr     r2, r2, #0x2            @ Flash Memory Chip Disable
        str     r2, [r1, #oNFCONT]
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	@judge if in steppingstone
	mov	r0, pc
	cmp	r0, #0x30000000
	bgt	jump_to_ram	@not in steppingstone, already in dram, go without copying nand code

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

        @ get ready to call C functions (for nand_read())
        @ldr     sp, DW_STACK_START      @ has been setuped at start
        mov     fp, #0                  @ no previous frame, so fp=0

#define TEXT_RAM_BASE   0x30000000
#define TEXT_NAND_BASE  0x0
#define TEXT_SIZE       0x100000

        @ copy
        ldr     r0, =TEXT_RAM_BASE
        ldr     r1, =TEXT_NAND_BASE
        ldr     r2, =TEXT_SIZE
        bl      nand_read_ll

        tst     r0, #0x0
        bne     bad_nand_read

        @ verify
        mov     r0, #0
        ldr     r1, =TEXT_NAND_BASE
        mov     r2, #0x400      @ 4 bytes * 1024 = 4K-bytes
go_next:
        ldr     r3, [r0], #4
        ldr     r4, [r1], #4
        teq     r3, r4
        bne     bad_nand_read
        subs    r2, r2, #4
        bne     go_next
	
	@jump to ram to run
jump_to_ram:
	ldr     r1, =here_is_ram
        add     pc, r1, #0
        nop
        nop
        b       .              @ infinite loop
bad_nand_read:
        mov    	r0, #'E'
	bl	s3c2440_serial_send_byte
	b	.

here_is_ram:
        b       main
                                                    
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
                                           
/*****************************************************************************/
