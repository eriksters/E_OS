#include "EOS.h"
#include "EOS_Mutex.h"
#include "EOS_Setup.h"
#include "EOS_Tasks.h"
#include "EOS_Scheduling.h"
#include "EOS_Blocking.h"

#include <stdio.h>

/* Handler for SVC interrupt, used for System Calls
 * Called from EOS_ASM.s
*/
void SVC_Handler_c( os_StackedReg_t* stackedRegisters );


void SVC_Handler_c( os_StackedReg_t* stackedRegisters ) {
	
	uint32_t ret = 0;
	
	uint8_t* SCN_p = (uint8_t*) stackedRegisters->PC;
	SCN_p -= 2;
	
	uint8_t SCN = *SCN_p;
	
	switch (SCN) {
		
		//	Start
		case 0:
			os_start_call_handler();
			break;
		
		//	Release 
		case 1:
			os_release_call_handler();
			break;
		
		//	Task Create
		case 2:
			ret = (uint32_t) os_create_task_call_handler( (void (*) (void *)) stackedRegisters->R0, (os_TCB_t*) stackedRegisters->R1, (void *) stackedRegisters->R2 );
			break;
		
		//	Delay
		case 3:
			os_delay_call_handler( stackedRegisters->R0 );
			break;
		
		//	Mutex Create
		case 4:
			ret = (uint32_t) os_create_mutex_call_handler( (os_mutex_t*) stackedRegisters->R0 );
			break;
		
		//	Mutex Lock
		case 5:
			ret = os_lock_mutex_call_handler( (os_mutex_h) stackedRegisters->R0 );
			break;
		
		//	Mutex Unlock
		case 6:
			ret = os_unlock_mutex_call_handler( (os_mutex_h) stackedRegisters->R0 );
			break;
		
		//	Task Delete
		case 7:
			os_delete_task_call_handler( (os_task_h) stackedRegisters->R0 );
			break;
		
		//	Task Finished (Automatically called when a task finishes)
		case 8:
			os_task_finished_call_handler();
			break;
		
		//	Shut Down OS
		case 9:
			os_exit_call_handler();
			break;
		
		//	Block a task for undetermined amount of time
		case 10:
			os_block_call_handler();
			break;
		
		//	Unblock a blocked task
		case 11:
			os_unblock_call_handler( (os_task_h) stackedRegisters->R0 );
			break;
	}
	
	stackedRegisters->R0 = ret;
}

void os_init( uint32_t tick_frq ) {
	os_init_call_handler( tick_frq );
} 

void os_start ( void ) {
	os_set_state( OS_STATE_STARTING );
	__asm("SVC #0x00");
}

void os_release ( void ) {
	__asm("SVC #0x01");
}

os_task_h os_create_task( void ( *func )( void * ), os_TCB_t* tcb, void * params ) {
	os_task_h ret;
	UNUSED(func);
	UNUSED(tcb);
	UNUSED(params);
	__asm("SVC #0x02");
	__asm("mov %0, r0" : "=r" (ret));
	return ret;
} 


void os_delay( uint32_t milliseconds ) {
	UNUSED(milliseconds);
	__asm("SVC #0x03");
}

os_mutex_h os_create_mutex( os_mutex_t* mutex_p ) {
	os_mutex_h ret;
	UNUSED(mutex_p);
	__asm("SVC #0x04");
	__asm("mov %0, r0" : "=r" (ret));
	return ret;
}

uint32_t os_lock_mutex( os_mutex_t* mutex_p ) {
	uint32_t ret;
	UNUSED(mutex_p);
	__asm("SVC #0x05");
	__asm("mov %0, r0" : "=r" (ret));
	return ret;
}

uint32_t os_unlock_mutex( os_mutex_t* mutex_p ) {
	uint32_t ret;
	UNUSED(mutex_p);
	__asm("SVC #0x06");
	__asm("mov %0, r0" : "=r" (ret));
	return ret;
}

void os_delete_task ( os_task_h task ) {
	UNUSED(task);
	__asm("SVC #0x07");
}


void os_task_finished ( void ) {
	__asm("SVC #0x08");
}

void os_exit( void ) {
	__asm("SVC #0x09");
}

void os_block( void ) {
	__asm("SVC #0x10");
}


void os_unblock( os_task_h task ) {
	UNUSED(task);
	__asm("SVC #0x11");
}
