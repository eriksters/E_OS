#include "EOS.h"
#include "EOS_SysCalls.h"
#include "EOS_Dispatcher.h"
#include "stm32f10x.h"
#include "EOS_Scheduler.h"

#include <stdio.h>

void SVC_Handler_f( os_StackedReg_t* stackedRegisters ) {
	uint16_t* SCI_p = (uint16_t*) stackedRegisters->PC;
	
	uint16_t SCI = *(--SCI_p);
	SCI &= 0xFF;
	
	switch (SCI) {
		
		//	Start
		case 0:
			os_Start_f();
			break;
		
		//	Release 
		case 1:
			os_Release_f();
			break;
		
		//	Delay
		case 3:
			os_Delay_f( stackedRegisters->R0 );
			break;
		
	}
}

void os_DeleteTask ( os_TCB_t * tcb ) {
	tcb = tcb + 1;
}

void os_Start ( void ) {
	__asm("SVC #0x00");
}

void os_Release ( void ) {
	__asm("SVC #0x01");
}

void os_TaskEnd ( void ) {
	__asm("SVC #0x02");
}

void os_Delay( uint32_t milliseconds ) {
	UNUSED(milliseconds);
	__asm("SVC #0x3");
}


