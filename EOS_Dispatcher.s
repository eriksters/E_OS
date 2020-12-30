				AREA 	|.text|, CODE, READONLY
	
				EXPORT	os_reg_Restore
				EXPORT	os_reg_Save
				EXPORT  SVC_Handler
				EXPORT	PendSV_Handler
	
				IMPORT 	os_start_f
				IMPORT	os_release_f
				IMPORT  SVC_Handler_f
				IMPORT  os_switch_f
				IMPORT  os_getCurrentTaskRegisters
				IMPORT 	os_GetStatus
				IMPORT  os_SetStarted



PendSV_Handler	PROC
	
				PUSH	{lr}
				
				BL		os_GetStatus
				CMP		r0, #0x0
				BEQ		Pend_EXIT
				CMP		r0, #0x3
				BNE		Pend_Started
				
				BL		os_switch_f
				BL		os_reg_Restore
				B		Pend_Set_Start
				
Pend_Started	BL		os_reg_Save
				BL		os_switch_f
				BL		os_reg_Restore
				
Pend_Set_Start	BL		os_SetStarted
Pend_EXIT		POP		{pc}
	
				ENDP
				
	
	
	
SVC_Handler		PROC
				
				;	If OS is started, set EXC_RETURN to return to unpriv thread mode,
				;	Otherwise, keep generated EXC_RETURN value
				
				PUSH	{lr}
				BL		os_GetStatus
				POP		{lr}
				
				CMP		r0, #3
				BEQ		SVC_os_starting				;	OS is starting
				PUSH	{lr}
				B		SVC_pushed_lr				;	OS not starting
SVC_os_starting	MOV		r1, #0xFFFFFFFD
				PUSH	{r1}	
				
				
SVC_pushed_lr	AND		r0, lr, #0x4				;	Check which stack was used for stacking
				CMP		r0, #0x4
				BEQ		SVC_Used_PSP
				MRS		r0, MSP
				ADD		r0, r0, #4
				B		SVC_Has_SP
SVC_Used_PSP	MRS		r0, PSP
				

				;	----	Do the rest of the ISR in C 	-----
SVC_Has_SP		BL		SVC_Handler_f
				
				POP		{pc}
				
				ENDP
	
	
	
	
os_reg_Save		PROC
				
				PUSH	{lr}
				
				BL		os_getCurrentTaskRegisters
				
				STR		r4, [r0]
				STR		r5, [r0, #0x4]
				STR		r6, [r0, #0x8]
				STR		r7, [r0, #0xC]
				STR		r8, [r0, #0x10]
				STR		r9, [r0, #0x14]
				STR		r10, [r0, #0x18]
				STR		r11, [r0, #0x1C]
				
				MRS		r1, PSP
				STR		r1, [r0, #0x20]
			
				POP		{pc}
				
				ENDP

	
	
	
os_reg_Restore	PROC
				
				PUSH	{lr}
				
				BL		os_getCurrentTaskRegisters
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
				
				POP		{pc}
								
				ENDP


				END