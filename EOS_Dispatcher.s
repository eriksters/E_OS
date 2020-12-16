				AREA 	|.text|, CODE, READONLY
	
				EXPORT	os_Reg_Restore
				EXPORT	os_Reg_Save
				EXPORT  SVC_Handler
				EXPORT	PendSV_Handler
	
				IMPORT	os_Control
				IMPORT 	os_Start_f
				IMPORT	os_Release_f
				IMPORT  SVC_Handler_f
				IMPORT  os_Switch_f
				IMPORT  os_getCurrentTask


PendSV_Handler	PROC
	
				PUSH	{lr}
				
				BL		os_Reg_Save
				BL		os_Switch_f
				CMP		r0, #0x1
				BEQ		PendSV_EXIT
				BL		os_Reg_Restore
				
PendSV_EXIT		POP		{pc}
	
				ENDP
				
	
SVC_Handler		PROC
				
				AND		r0, lr, #0x4				;	Check which stack was used for stacking
				CMP		r0, #0x4
				BEQ		Used_PSP
				MRS		r0, MSP
				B		Has_SP
Used_PSP		MRS		r0, PSP


				;	----	Do the rest of the ISR in C 	-----

Has_SP			MOV		r1, #0xFFFFFFFD
				PUSH	{r1}
				
				BL		SVC_Handler_f
				
				POP		{pc}
				
				ENDP
	
	
os_Reg_Save		PROC
				
				PUSH	{lr}
				
				BL		os_getCurrentTask
			;	LDR		r0, [r0, #0x04]			;	Load address of Task Register struct
				
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

	
os_Reg_Restore	PROC
				
		;		MOV 	r0, #0xFFFFFFFD 
		;		PUSH 	{r0}					;	Set EXC_RETURN to return to thread mode and use PSP
				
				PUSH	{lr}
				
				BL		os_getCurrentTask
				
		;		LDR		r0, =os_Control
		;		LDR		r0, [r0, #0x04]			;	Load address of Task Register struct
				
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