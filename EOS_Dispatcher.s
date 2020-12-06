				AREA 	|.text|, CODE, READONLY
	
				EXPORT	os_Reg_Restore
				EXPORT	os_Reg_Save
				EXPORT  SVC_Handler
	
				IMPORT	os_Control
				IMPORT 	os_Start_f
				IMPORT	os_Release_f
	
	
SVC_Handler		PROC
			
				CMP		lr, #0xFFFFFFF9			;	Check which SP was used when calling SVC
				BNE		Used_PSP
				MOV		r0, sp
				B		Has_SP
				
				
				;	----	Find ID of SVC Call		-----
Used_PSP		MRS		r0, PSP

Has_SP			ADD		r0, #0x18
				LDR		r1, [r0]				; 	Stacked SP
				
				SUB		r1, #0x2				; 	SVC instruction address
				LDRH	r0, [r1]
				AND		r0, #0xFF				; 	Get rid of all other bits that are not call ID
			
				MOV		r1, #0xFFFFFFFD
				PUSH	{r1}
				
				CMP		r0, #0x00
				BLEQ	os_Start_f
				
				CMP		r0, #0x01
				BLEQ	os_Release_f
				
				POP		{pc}
				
				ENDP
	
	
os_Reg_Save		PROC
				
				LDR		r0, =os_Control
				LDR		r0, [r0, #0x04]			;	Load address of Task Register struct
				
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
				
				BX		lr
				
				ENDP

	
os_Reg_Restore	PROC
				
		;		MOV 	r0, #0xFFFFFFFD 
		;		PUSH 	{r0}					;	Set EXC_RETURN to return to thread mode and use PSP
				
				LDR		r0, =os_Control
				LDR		r0, [r0, #0x04]			;	Load address of Task Register struct
				
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




				END