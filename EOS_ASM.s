#include "EOS_Defines.h"

				AREA 	|.text|, CODE, READONLY
	
				EXPORT	os_reg_restore
				EXPORT	os_reg_save
				EXPORT  SVC_Handler
				EXPORT	PendSV_Handler
	
				IMPORT 	os_start_call_handler
				IMPORT	os_release_call_handler
				IMPORT  SVC_Handler_c
				IMPORT  os_switch_current_task
				IMPORT  os_get_current_task_reg
				IMPORT 	os_get_state
				IMPORT  os_set_state
				IMPORT  os_set_current_task_state

PendSV_Handler	PROC
	
				PUSH	{lr}
				
				BL		os_get_state
				
				CMP		r0, #OS_STATE_RUNNING	;	OS is running, so schedule normally
				BEQ		Pend_Normal
				
				CMP		r0, #OS_STATE_STARTING	;	OS is starting
				BEQ		Pend_Starting			
				
				CMP		r0, #OS_STATE_EXIT		;	OS is shutting down
				BEQ		Pend_Exit
				
				B		Pend_Pop				;	OS is not in a schedulable state
				
				
Pend_Normal		BL		os_reg_save				
				BL		os_switch_current_task
				BL		os_reg_restore
				B		Pend_Run

Pend_Exit		POP		{r0}

				MOV		r0, #0x00
				MSR		CONTROL, r0
				ISB

				MOV		r0, #0xFFFFFFF9
				PUSH	{r0}
				
				B		Pend_Pop
				
				
Pend_Starting	BL		os_switch_current_task				;	OS is starting, so set os status and do not back up registers
				BL		os_reg_restore
				MOV		r0, #OS_STATE_RUNNING
				BL		os_set_state


Pend_Run		MOV		r0, #OS_TASK_STATE_RUNNING
				BL		os_set_current_task_state
Pend_Pop		POP		{pc}
	
				ENDP
				
	
	
	
SVC_Handler		PROC
				
				;	If OS is started, set EXC_RETURN to return to unpriv thread mode,
				;	Otherwise, keep generated EXC_RETURN value
				
				PUSH	{lr}
				BL		os_get_state
				POP		{lr}
				
				CMP		r0, #OS_STATE_STARTING
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
SVC_Has_SP		BL		SVC_Handler_c
				
				POP		{pc}
				
				ENDP
	
	
	
	
os_reg_save		PROC
				
				PUSH	{lr}
				
				BL		os_get_current_task_reg
				
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

	
	
	
os_reg_restore	PROC
				
				PUSH	{lr}
				
				BL		os_get_current_task_reg
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