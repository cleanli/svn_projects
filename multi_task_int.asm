; .\t.SRC generated from: t.c
; COMPILER INVOKED BY:
;        C:\Keil\C51\BIN\C51.EXE t.c LARGE BROWSE DEBUG OBJECTEXTEND SRC(.\t.SRC)


NAME	T

?PR?Timer0?T         SEGMENT CODE 
	EXTRN	XDATA (task_list)
	EXTRN	XDATA (cur_task_index)
	EXTRN   XDATA (total_task)
	EXTRN	CODE (task1)
	PUBLIC	Timer0
; #include "global.h"
; 
CSEG	AT	0000BH
	LJMP	Timer0

; void Timer0(void) interrupt 1 using 0

	RSEG  ?PR?Timer0?T
	USING	0
Timer0:
	PUSH 	ACC
	PUSH 	B
	PUSH 	DPH
	PUSH 	DPL
	PUSH 	PSW
			; SOURCE LINE # 3
; {
; task_list[cur_task_index].task_fun=task1;
			; SOURCE LINE # 5
; task_list[cur_task_index].task_fun=task1;
			; SOURCE LINE # 5
    LCALL   get_addr
	MOV     B,AR0     ;bak r0
	MOV     R0,#7FH
loop1:
	MOV  	A,@R0
	MOVX 	@DPTR,A
	INC  	DPTR
	DJNZ    R0,loop1   ;7f-01 stored
	MOV     A,B
        MOVX 	@DPTR,A    ;r0 stored

	MOV     R0,#5
loop2:
	INC  	DPTR
	POP     ACC
	MOVX    @DPTR,A
	DJNZ    R0,loop2   ;psw dpl dph b acc stored

	POP     AR0    ;current PC high
	POP     AR1    ;PC low baked

	INC     DPTR
	MOV     A,SP
	MOVX    @DPTR,A ;sp stored

	INC     DPTR
        mov     a,#0xff
        movx    @dptr,a
	INC     DPTR    ;skip the first byte
	MOV     A,R0
	MOVX    @DPTR,A
	INC     DPTR
	MOV     A,R1
	MOVX    @DPTR,A ;current PC stored

;   cur_task_index++;
	MOV  	DPTR,#cur_task_index
	MOVX 	A,@DPTR
	INC  	A
	MOVX 	@DPTR,A
; 	if(cur_task_index==total_task)cur_task_index=0;
	MOV  	DPTR,#total_task
	MOVX 	A,@DPTR
	MOV  	R7,A
	MOV  	DPTR,#cur_task_index
	MOVX 	A,@DPTR
	CJNE 	A,AR7,?C0002
	CLR  	A
	MOVX 	@DPTR,A

?C0002:
    LCALL   get_addr
	MOV     R6,DPL
	MOV     R7,DPH    ;bak THE ADDR
	MOV     A,DPL
	ADD     A,#0x88    ;last byte of task_fun
	MOV     DPL,A
	MOV     A,DPH
	ADDC    A,#0
	MOV     DPH,A      ;addr ready

	MOVX    A,@DPTR
	MOV     R0,A
	LCALL   dec_dptr
	MOVX    A,@DPTR
	MOV     R1,A

	LCALL   dec_dptr
	LCALL   dec_dptr
	MOVX    A,@DPTR
	MOV     SP,A        ;sp restored

	PUSH    AR0
	PUSH    AR1          ;PC in stack
 
	MOV     R0,#5
loop3:
	LCALL   dec_dptr
	MOVX    A,@DPTR
	PUSH    ACC
	DJNZ    R0,loop3      ;psw dpl dph b acc restored

	LCALL   dec_dptr
	MOVX    A,@DPTR
	MOV     B,A     ;AR0 baked
	MOV     DPH,R7
	MOV     DPL,R6  ;first byte of struct
	MOV     R0,#0x7F
loop4:

	MOVX 	A,@DPTR
	MOV  	@R0,A
	INC  	DPTR
	DJNZ    R0,loop4
	MOV     AR0,B   ;00h restored

	POP  	PSW
	POP  	DPL
	POP  	DPH
	POP  	B
	POP  	ACC
	CLR     TF0
	MOV     TL0,TH0
	RETI 	

get_addr:
	MOV  	DPTR,#cur_task_index
	MOVX 	A,@DPTR
	MOV  	B,#08AH
	MUL  	AB
	ADD  	A,#LOW (task_list)
	MOV  	DPL,A
	MOV  	A,B
	ADDC 	A,#HIGH (task_list)
	MOV  	DPH,A
	RET
dec_dptr:
    MOV     A,DPL
	CLR     C
	SUBB     A,#1
	MOV     DPL,A
	MOV     A,DPH
	SUBB    A,#0
	MOV     DPH,A
	RET
	END
