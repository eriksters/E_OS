#include "EOS.h"
#include "EOS_SysCalls.h"
#include "EOS_Scheduler.h"
#include "EOS_Control.h"
#include "EOS_DataStructures.h"
#include <stdio.h>

void os_init( uint32_t os_tick_frq ) {
	os_Control.state = OS_STATE_INIT;

	os_core_init( os_tick_frq );
	os_scheduler_init();
	
	os_Control.state = OS_STATE_POST_INIT;
}


os_task_h os_task_create_f ( void ( *func )( void * ), os_TCB_t* tcb, void * params ) {
	
	//	Stack is descending, so Stack Pointer must be set to end of the memory block
	//	The EXC_RETURN automatically restores several registers, which are stored on the task stack,
	//	so the SP is adjusted to make sure this happens correctly
	
	os_task_h ret = 0;
	
	if ( os_Control.taskCount < OS_MAX_TASK_COUNT ) {
		uint32_t* stack = (uint32_t*) tcb->stack;
		
		os_arch_create_task( func, (uint32_t*) stack, &tcb->backed_up_registers, params );
		
		os_schedule_task( tcb );
		
		os_Control.taskCount++;
		
		ret = (os_task_h) tcb;
	}
	
	return ret;
}

void os_task_delete_f ( os_task_h task ) {
	
	uint32_t mutexArraySize = 0;
	os_mutex_t* mutex_p = 0;
	uint32_t isCurrentTask = ( task == 0 );
	
	//	Check if calling task or another task
	if ( isCurrentTask ) {
		task = os_ctrl_get_current_task();
	}
	
	//	Schedule for removal
	os_deschedule_task( task );

	
	//	Release resources
	mutexArraySize = os_arrayList_size( os_mutex_arraylist_handle );
	for ( uint32_t i = 0; i < mutexArraySize; i++ ) {
		mutex_p = os_arrayList_get( os_mutex_arraylist_handle, i );
		if ( mutex_p->owner == task ) {
			mutex_p->owner = 0;
		}
	}
	
	if ( isCurrentTask ) {
		os_task_switch_trigger();
	}
	
}

void os_start_f( void ) {
	
	printf("OS Start \n");
	os_Control.state = OS_STATE_STARTING;
	
	//	Do not start if no tasks have been created
	if (os_Control.taskCount < 1) {
		return;
	}
	
	//	Set up hardware (stack pointer selection and interrupt priorities)
	os_arch_start();
		
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
	
	os_task_h task = os_ctrl_get_current_task();
	
	os_block_task( task );
	
	task->countdown = milliseconds / 10;
	
	os_tick_reset();
	
	os_task_switch_trigger();
}


void os_task_end_f ( void ) {
	
	os_task_delete_f( 0 );
	os_task_switch_trigger();
	
}


uint32_t os_mutex_create_f( os_mutex_t* mutex_p ) {
	
	mutex_p->owner = 0;
	
	//	Error if mutex is already initialized
	if ( os_arrayList_contains( os_mutex_arraylist_handle, mutex_p ) ) {
		return 1;
	}
	
	//	Error if mutex can not be added to mutex list
	if (os_arrayList_add( os_mutex_arraylist_handle, mutex_p )) {
		return 1;
	}
	
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

void os_exit_f( void ) {
	os_Control.state = OS_STATE_EXIT;
	os_task_switch_trigger();
}



