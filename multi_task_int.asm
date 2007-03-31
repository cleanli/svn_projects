; .\t.SRC generated from: t.c
; COMPILER INVOKED BY:
;        C:\Keil\C51\BIN\C51.EXE t.c LARGE BROWSE DEBUG OBJECTEXTEND SRC(.\t.SRC)


NAME	T

?PR?Timer0?T         SEGMENT CODE 
        EXTRN	XDATA (soft_int)
	EXTRN	XDATA (intrpt_count)
	EXTRN	XDATA (task_head)
	EXTRN	XDATA (cur_task)
		
	PUBLIC	Timer0


; 
CSEG	AT	0000BH
	LJMP	Timer0

; void Timer0(void) interrupt 1 using 0

	RSEG  ?PR?Timer0?T
	USING	1
Timer0: 
	PUSH 	ACC
	PUSH 	DPH
	PUSH 	DPL
	PUSH 	PSW
	USING	1
	MOV  	PSW,#08H
			; SOURCE LINE # 3
; {
; task_list[cur_task_index].task_fun=task1;
			; SOURCE LINE # 5
; task_list[cur_task_index].task_fun=task1;
			; SOURCE LINE # 5
	MOV  	R0,#LOW (soft_int)
	MOVX 	A,@R0
	JZ   	real_int
; 	    soft_int=0;
			; SOURCE LINE # 330
	CLR  	A
	MOVX 	@R0,A
	SJMP 	end_judge1
real_int:
			; SOURCE LINE # 331
; 	    intrpt_count++;
			; SOURCE LINE # 332
	MOV  	R0,#LOW (intrpt_count)
	MOVX 	A,@R0
	INC  	A
	MOVX 	@R0,A
; 		TH0=TIMER;
			; SOURCE LINE # 333
	MOV  	TH0,#0E0H
; 		TL0=0;
			; SOURCE LINE # 334
	;MOV  	TL0,#00H
; 	}
end_judge1:
        LCALL   new_get_addr
        MOV     R6,DPL
        MOV     R7,DPH
        MOV     R1,#0x0
	MOV     R2,#0x80
loop1:
	MOV  	A,@R1
	MOVX 	@DPTR,A
	INC  	DPTR
	INC     R1
	DJNZ    R2,loop1   ;7f-01 stored

        MOV     DPL,R6
        MOV     DPH,R7
        MOV     A,#0x85    ;->reg_bak.sp
        LCALL   add_dptr   
        MOV     A,SP
        MOVX    @DPTR,A    ;SP stored

next_task:
        LCALL   new_get_addr
        MOV     A,#0x8C ;->next_task
        LCALL   add_dptr  
	MOV  	R0,#LOW (cur_task)
	MOVX 	A,@DPTR
	MOV  	R5,A
	MOVX     @R0,A
	INC  	DPTR
	INC     R0
	MOVX 	A,@DPTR
	MOVX     @R0,A
	ORL  	A,R5
	JNZ  	not_end_task
	MOV  	R0,#LOW (task_head)
	MOVX 	A,@R0
	MOV  	R7,A
	INC  	R0
	MOVX 	A,@R0
	MOV  	R0,#LOW (cur_task)
	XCH  	A,R7
	MOVX 	@R0,A
	INC  	R0
	MOV  	A,R7
	MOVX 	@R0,A
not_end_task:

	LCALL   new_get_addr
	MOV     R6,DPL
        MOV     R7,DPH
	MOV     A,#0x88    ;->.status
	LCALL   add_dptr
	MOVX     A,@DPTR
	JNZ     next_task

        MOV     DPL,R6
        MOV     DPH,R7
        MOV     A,#0x85
        LCALL   add_dptr   
        MOVX    A,@DPTR   ;SP restored
	MOV     SP,A

	MOV     DPL,R6
        MOV     DPH,R7
        MOV     R1,#0x0
	MOV     R2,#0x08
loop2:
	MOVX 	A,@DPTR
	MOV  	@R1,A
	INC  	DPTR
	INC     R1
	DJNZ    R2,loop2   ;01-08 stored

	MOV     A,#08
	LCALL   add_dptr
        MOV     R1,#0x10
	MOV     R2,#0x70
loop3:
	MOVX 	A,@DPTR
	MOV  	@R1,A
	INC  	DPTR
	INC     R1
	DJNZ    R2,loop3   ;10-7f restored




	POP  	PSW
	POP  	DPL
	POP  	DPH
	POP  	ACC
	RETI 

new_get_addr:
	MOV  	R0,#LOW (cur_task)
	MOVX  	A,@R0
	MOV     DPH,A
	INC  	R0
	MOVX 	A,@R0
	MOV     DPL,A
	RET

add_dptr:
        MOV     R2,DPL
	ADD     A,R2    ;last byte of task_fun
	MOV     DPL,A
	MOV     A,DPH
	ADDC    A,#0
	MOV     DPH,A 
        RET
        
	END
