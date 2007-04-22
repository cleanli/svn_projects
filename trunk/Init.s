;*******************************************
;Institute of Automation, Chinese Academy of Sciences
;file Name:  Init.s
;Description:
;Author:   LijuGuang
;*******************************************

	IMPORT Main
	AREA Init, CODE, READONLY
	ENTRY
	
	LDR R0, =0x3ff0000
	LDR R1, =0xe7ffff80
	STR R1, [R0]
	LDR SP, = 0x3fe1000
	BL Main
	B .
	END