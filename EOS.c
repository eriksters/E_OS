#include "EOS.h"
#include "EOS_SysCalls.h"
#include "EOS_Dispatcher.h"
#include "stm32f10x.h"
#include "EOS_Scheduler.h"

#include <stdio.h>

extern os_Control_t os_Control;

void SVC_Handler_f( os_StackedReg_t* stackedRegisters ) {
	
	uint32_t ret = 0;
	
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
		
		//	Mutex Create
		case 4:
			ret = os_mutex_create_f( (os_mutex_t*) stackedRegisters->R0 );
			break;
		
		//	Mutex Lock
		case 5:
			ret = os_mutex_lock_f( (os_mutex_t*) stackedRegisters->R0 );
			break;
		
		//	Mutex Unlock
		case 6:
			ret = os_mutex_unlock_f( (os_mutex_t*) stackedRegisters->R0 );
			break;
		
	}
	
	stackedRegisters->R0 = ret;
}

void os_DeleteTask ( os_TCB_t * tcb ) {
	tcb = tcb + 1;
}

void os_Start ( void ) {
	os_Control.status = starting;
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
	__asm("SVC #0x03");
}

uint32_t os_mutex_create( os_mutex_t* mutex_p ) {
	uint32_t ret;
	UNUSED(mutex_p);
	__asm("SVC #0x04");
	__asm("mov %0, r0" : "=r" (ret));
	return ret;
}

uint32_t os_mutex_lock( os_mutex_t* mutex_p ) {
	uint32_t ret;
	UNUSED(mutex_p);
	__asm("SVC #0x05");
	__asm("mov %0, r0" : "=r" (ret));
	return ret;
}

uint32_t os_mutex_unlock( os_mutex_t* mutex_p ) {
	uint32_t ret;
	UNUSED(mutex_p);
	__asm("SVC #0x06");
	__asm("mov %0, r0" : "=r" (ret));
	return ret;
}


