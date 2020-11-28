
				AREA 	|.text|, CODE, READONLY
				
				EXPORT 	Sys_Call
				EXPORT 	SVC_Handler
					
SVC_Handler		PROC
	
				;	Disable all exceptions (except NMI and HardFault)
				MOV		r0, #0x0
				MSR		primask, r0
				
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
				
				
				
				
				BX		lr
				ENDP
					
Sys_Call		PROC
				SVC		#0xAA
				BX		lr
				ENDP
				
				END