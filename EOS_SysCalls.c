#include "EOS.h"
#include "EOS_SysCalls.h"
#include "EOS_Scheduler.h"
#include "EOS_Control.h"
#include <stdio.h>

void os_init( uint32_t os_tick_frq ) {
	os_Control.status = init;

	os_core_init( os_tick_frq );
	os_scheduler_init();
	
	os_Control.status = post_init;
}

//	TODO: Implement with SCI
void os_task_create_f ( void ( *func )( void * ), os_TCB_t* tcb, void * params ) {
	
	//	Stack is descending, so Stack Pointer must be set to end of the memory block
	//	The EXC_RETURN automatically restores several registers, which are stored on the task stack,
	//	so the SP is adjusted to make sure this happens correctly
	
	uint32_t* stack = (uint32_t*) tcb->stack;
	
	os_arch_create_task( func, (uint32_t*) stack, &tcb->backed_up_registers, params );
	
	
	os_ready_add( tcb );
	//os_Task_Queue[os_Control.taskCount] = tcb;
	os_Control.taskCount++;
}

void os_task_delete_f ( os_TCB_t* tcb ) {
	
	//	Check if calling task or another task
	tcb = 0;
	
	//	Remove from scheduling
	
	//	Remove from blocked queue
	
	//	Release resources
	
}

void os_start_f( void ) {
	
	printf("OS Start \n");
	os_Control.status = starting;
	
	//	Do not start if no tasks have been created
	if (os_Control.taskCount < 1) {
		return;
	}
	
	//	Set System to use PSP in thread mode
	__set_CONTROL(0x02);
		
	//	Start the OS
	os_task_switch_trigger();
}

void os_release_f ( void ) {
	printf("OS Release\n");
	
	os_tick_reset();
	
	os_task_switch_trigger();
}

void os_delay_f( uint32_t milliseconds ) {
	printf("Milliseconds: %d\n", milliseconds);
	
	os_TCB_t* task = os_ctrl_get_current_task();
	
	os_blocked_add( task );
	
	task->countdown = milliseconds / 10;
	
	os_Control.status = block;
	
	os_tick_reset();
	
	os_task_switch_trigger();
}

void os_task_end_f ( void ) {

}

uint32_t os_mutex_create_f( os_mutex_t* mutex_p ) {
	UNUSED(mutex_p);
	return 0;
}

uint32_t os_mutex_lock_f( os_mutex_t* mutex_p ) {

	//	If Mutex is locked by a task, return 1
	if ( mutex_p->owner != 0 ) {
		return 1;
	}
	
	//	Assign Mutex's owner to calling task
	mutex_p->owner = os_ctrl_get_current_task();
	
	return 0;
}

uint32_t os_mutex_unlock_f( os_mutex_t* mutex_p ) {
	
	//	If task is locked by another thread or not locked at all, return 1
	if ( mutex_p->owner != os_ctrl_get_current_task() ) {
		return 1;
	}
	
	//	Unlock the thread
	mutex_p->owner = 0;
	
	return 0;
}




