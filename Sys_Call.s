
				AREA 	|.text|, CODE, READONLY
				
				EXPORT 	Sys_Call
				EXPORT 	SVC_Handler
				EXPORT	SwitchPSP
				EXPORT  start_task
;				EXPORT	os_start
				EXPORT	os_switch
				EXPORT 	os_dispatch

SVC_Handler		PROC
				
				MOV		r0, 0x3
				MSR		CONTROL, r0
				MOV 	r0, lr
				AND		r0, #0xFFFFFFF0 
				ORR		r0, #0xD
				PUSH 	{r0}
				MOV		r0, sp
				MSR 	PSP, r0
				
				IMPORT	control
				IMPORT	task_queue
				
				LDR		r0, =task_queue
				LDR		r0, [r0]
		;		LDR		r1, =control
		;		LDR		r1, [r1]
		;		ADD		r0, r0, r1
				
				; os_dispatch(task_queue[control.currentTask]);
				
				BL		os_dispatch
	
				POP	{pc}

				ENDP
					

os_dispatch		PROC							;	r0 - pointer to desired task registers
				
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
				BX		lr
	
				ENDP
					
os_switch		PROC							;	r0 = pointer to current task, r1 = pointer to target task
	
				STR		r0, [r0]
				STR		r1, [r0, #0x4]
				STR		r2, [r0, #0x8]
				STR		r3, [r0, #0xC]
				STR		r4, [r0, #0x10]
				STR		r5, [r0, #0x14]
				STR		r6, [r0, #0x18]
				STR		r7, [r0, #0x1C]
				STR		r8, [r0, #0x20]
				STR		r9, [r0, #0x24]
				STR		r10, [r0, #0x28]
				STR		r11, [r0, #0x2C]
				STR		r12, [r0, #0x30]
				STR		sp, [r0, #0x34]
				STR		lr, [r0, #0x38]
		;		STR		pc, [r0, #0x3C]			;	PC?
		
				MRS		r2, xPSR
				STR		r2, [r0, #0x40]
				
				MOV		r0, r1
				
				B		os_dispatch
	
				ENDP

;os_start		PROC
;	
;				EXTERN	task_queue			
;				LDR		r0, =task_queue			;	Queue
;				LDR		r0, [r0]				; 	First Task In Queue
;				
;				B		os_dispatch
;				
;				ENDP
				
;os_dispatch 	PROC							;	r0 = task to load
;	
;				;	Load xPSR
;				LDR		r1, [r0, #0x40]
;				MSR		xPSR, r1
;				
;				;	Load SP
;				LDR		sp, [r0, #0x34]
;				
;				;	Push Program Counter so it can be popped later
;				LDR		r1, [r0, #0x3C]
;				PUSH	{r1}
;				
;				;	Load Register Bank
;				LDR		r1, [r0, #0x4]
;				LDR		r2, [r0, #0x8]
;				LDR		r3, [r0, #0xC]
;				LDR		r4, [r0, #0x10]
;				LDR		r5, [r0, #0x14]
;				LDR		r6, [r0, #0x18]
;				LDR		r7, [r0, #0x1C]
;				LDR		r8, [r0, #0x20]
;				LDR		r9, [r0, #0x24]
;				LDR		r10, [r0, #0x28]
;				LDR		r11, [r0, #0x2C]
;				LDR		r12, [r0, #0x30]
;				LDR		lr, [r0, #0x38]
;				
;				POP		{pc}
;	
;				ENDP
					
					
					
					
start_task		PROC	; Start an initialized task
	
				LDR		r1, [r0, #0x4]
				LDR		r2, [r0, #0x8]
				LDR		r3, [r0, #0xC]
				LDR		r4, [r0, #0x10]
				LDR		r5, [r0, #0x14]
				LDR		r6, [r0, #0x18]
				LDR		r7, [r0, #0x1C]
				LDR		r8, [r0, #0x20]
				LDR		r9, [r0, #0x24]
				LDR		r10, [r0, #0x28]
				LDR		r11, [r0, #0x2C]
				LDR		r12, [r0, #0x30]
				LDR		sp, [r0, #0x34]
				LDR		lr, [r0, #0x38]	
				LDR		pc, [r0, #0x3C]
	
				ENDP
					
					
					
Reg_Store		PROC	; Store register bank in registers struct
						; R0 must be pointer to struct
				
				STR		r4, [r0, #0x10]
				STR		r5, [r0, #0x14]
				STR		r6, [r0, #0x18]
				STR		r7, [r0, #0x1C]
				STR		r8, [r0, #0x20]
				STR		r9, [r0, #0x24]
				STR		r10, [r0, #0x28]
				STR		r11, [r0, #0x2C]
				STR		r12, [r0, #0x30]
				STR		sp, [r0, #0x34]
				STR		lr, [r0, #0x38]
				; STR		pc, [r0, #0x3C]
				MRS		r1, xPSR
				STR		r1, [r0, #0x40]
				
				BX		lr
				
				ENDP

SwitchPSP		PROC
				
				MOV 	r1, sp
				MOV		r0, #0x2
				MSR 	CONTROL, r0
				MOV		sp, r1
	
				ENDP
				

;SVC_Handler		PROC
;	
;				;	Disable all exceptions (except NMI and HardFault)
;				MOV		r0, #0x0
;				MSR		primask, r0
				
				;	Assert that this is a -RELEASE- call
				;	TODO
				
				
				;	Code for getting system call ID
				
	;			MOV		r0, sp
	;			ADD		r0, #0x18
	;			LDR		r1, [r0]	; Stacked SP
	;			
	;			SUB		r1, #0x2	; SVC instruction address
	;			LDRH	r0, [r1]
	;			AND		r0, #0xFF	; Get rid of all other bits that are not call ID
				
				
				
;				EXTERN	t1_reg
;				LDR		r0, =t1_reg
;				PUSH	{lr}
;				BL		Reg_Store
				
;				POP		{lr}
;				BX		lr
;				ENDP
					
Sys_Call		PROC
				SVC		#0xAA
				BX		lr
				ENDP
				
				END