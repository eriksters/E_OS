				AREA 	|.text|, CODE, READONLY
	
				EXPORT	os_Reg_Restore
				EXPORT  SVC_Handler
	
	
SVC_Handler		PROC
			
				IMPORT 	os_Start
				B		os_Start
				
				ENDP
	
	
	
os_Reg_Restore	PROC
				
				MOV 	r0, #0xFFFFFFFD 
				PUSH 	{r0}					;	Set EXC_RETURN to return to thread mode and use PSP
				
				IMPORT	os_Control
				IMPORT	os_Task_Queue
				
				LDR		r0, =os_Task_Queue
				LDR		r0, [r0]				;	Load address of task register struct
		;		LDR		r1, =control
		;		LDR		r1, [r1]
		;		ADD		r0, r0, r1
				
		; os_dispatch(task_queue[control.currentTask]);
				
				LDR		r4, [r0]
				LDR		r5, [r0, #0x4]
				LDR		r6, [r0, #0x8]
				LDR		r7, [r0, #0xC]
				LDR		r8, [r0, #0x10]
				LDR		r9, [r0, #0x14]
				LDR		r10, [r0, #0x18]
				LDR		r11, [r0, #0x1C]
				LDR		r0, [r0, #0x20]
				MSR		PSP, r0
				
				POP	{pc}
				
				ENDP




				END