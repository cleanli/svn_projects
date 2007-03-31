; .\t.SRC generated from: t.c
; COMPILER INVOKED BY:
;        C:\Keil\C51\BIN\C51.EXE t.c LARGE BROWSE DEBUG OBJECTEXTEND SRC(.\t.SRC)


NAME	T

?PR?Timer0?T         SEGMENT CODE 
        EXTRN	XDATA (is_timer0_int)
	EXTRN	XDATA (intrpt_count)
	PUBLIC	Timer0


; 
CSEG	AT	0000BH
	LJMP	Timer0

; void Timer0(void) interrupt 1 using 0

	RSEG  ?PR?Timer0?T
	USING	0
Timer0: 
        MOV     TH0,#TIMER
	PUSH    ACC
	MOV     ACC,#1
        LCALL   SET_TIMER_FLAG
        LCALL   TIMER_SWITCH_TASK

SET_TIMER_FLAG:
 	PUSH    DPH
	PUSH    DPL
	MOV  	DPTR,#is_timer0_int
	MOVX    @DPTR,A 

	MOV  	DPTR,#intrpt_count
	MOVX    A, @DPTR
        INC     A
        MOVX    @DPTR,A ;inc intrpt_count
        POP     DPL
	POP     DPH
	RET
TIMER_SWITCH_TASK:
        POP     ACC
	POP     ACC  ;DELETE THE PC VALUE WHERE LCALL THIS FUN
	POP     ACC  ;RESTORE ACC
	LJMP    switch_task




?PR?SWITCH_TASK         SEGMENT CODE 
	EXTRN	XDATA (task_head)
	EXTRN	XDATA (cur_task)
	PUBLIC	SWITCH_TASK
; #include "global.h"
        TIMER EQU 0xe0


	RSEG  ?PR?SWITCH_TASK
	USING	0
switch_task: 
        CLR     EA
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
        LCALL   new_get_addr
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

nexttask:
; 	cur_task=cur_task->next_task;
			; SOURCE LINE # 186
	MOV  	DPTR,#cur_task
	MOVX 	A,@DPTR
	MOV  	R6,A
	INC  	DPTR
	MOVX 	A,@DPTR
	ADD  	A,#08DH
	MOV  	DPL,A
	CLR  	A
	ADDC 	A,R6
	MOV  	DPH,A
	MOVX 	A,@DPTR
	MOV  	R6,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	R7,A
	MOV  	DPTR,#cur_task
	MOV  	A,R6
	MOVX 	@DPTR,A
	INC  	DPTR
	MOV  	A,R7
	MOVX 	@DPTR,A
; 	if(cur_task == NULL)cur_task=task_head;
			; SOURCE LINE # 187
	ORL  	A,R6
	JNZ  	L36
	MOV  	DPTR,#task_head
	MOVX 	A,@DPTR
	MOV  	R7,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV  	DPTR,#cur_task
	XCH  	A,R7
	MOVX 	@DPTR,A
	INC  	DPTR
	MOV  	A,R7
	MOVX 	@DPTR,A
L36:
; 	if(cur_task -> status)goto nexttask;
			; SOURCE LINE # 188
	MOV  	DPTR,#cur_task
	MOVX 	A,@DPTR
	MOV  	R7,A
	MOV     B,A
	INC  	DPTR
	MOVX 	A,@DPTR
	MOV     R6,A
	ADD  	A,#089H
	MOV  	DPL,A
	CLR  	A
	ADDC 	A,B
	MOV  	DPH,A
	MOVX 	A,@DPTR
	JNZ   	nexttask

	LCALL   dec_dptr

	MOVX    A,@DPTR   ;;last byte of task_fun
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

        PUSH    ACC
	PUSH    DPL
	PUSH    DPH
	MOV     DPTR,#is_timer0_int
	MOVX    A,@DPTR
	POP     DPH
	POP     DPL
	JNZ     FROM_TIMER0
	POP     ACC
	SETB     EA
	RET
FROM_TIMER0:
        MOV     ACC,#0
	LCALL   SET_TIMER_FLAG
	POP     ACC
	SETB    EA
	RETI 
















new_get_addr:
	MOV  	DPTR,#cur_task
	MOVX 	A,@DPTR
	MOV  	B,A
	INC     DPTR
	MOVX  	A,@DPTR
	MOV  	DPL,A
	MOV     A,B
	MOV     DPH,A
	RET

dec_dptr:
        MOV     A,DPL
	JZ      F1
	DEC     DPL
	RET
F1:     DEC     DPH
        MOV     DPL,0xff
	RET

add_dptr:
        MOV     B,DPL
	ADD     A,B    ;last byte of task_fun
	MOV     DPL,A
	MOV     A,DPH
	ADDC    A,#0
	MOV     DPH,A 
        RET
        
dec_dptr_old:
        MOV     A,DPL
	CLR     C
	SUBB     A,#1
	MOV     DPL,A
	MOV     A,DPH
	SUBB    A,#0
	MOV     DPH,A
	RET

	END
