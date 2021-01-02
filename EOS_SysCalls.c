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


void os_task_create_f ( void ( *func )( void * ), os_TCB_t* tcb, void * params ) {
	
	//	Stack is descending, so Stack Pointer must be set to end of the memory block
	//	The EXC_RETURN automatically restores several registers, which are stored on the task stack,
	//	so the SP is adjusted to make sure this happens correctly
	
	uint32_t* stack = (uint32_t*) tcb->stack;
	
	os_arch_create_task( func, (uint32_t*) stack, &tcb->backed_up_registers, params );
	
	if ( os_ready_add( tcb ) == tcb ) {
		tcb->state = OS_TASK_STATE_READY;
	} else {
		//	TODO: Test how this works
		tcb->state = OS_TASK_STATE_ZOMBIE;
	}
	
	os_Control.taskCount++;
}

void os_task_delete_f ( os_TCB_t* tcb ) {
	
	uint32_t mutexArraySize = 0;
	os_mutex_t* mutex_p = 0;
	uint32_t isCurrentTask = ( tcb == 0 );
	
	//	Check if calling task or another task
	if ( isCurrentTask ) {
		tcb = os_ctrl_get_current_task();
	}
	
	//	Remove from blocked
	if ( tcb->state == OS_TASK_STATE_BLOCKED ) {
		os_blocked_remove( tcb );
	}
	
	//	Set task state as Zombie and schedule for removal
	if ( tcb->state != OS_TASK_STATE_READY && tcb->state != OS_TASK_STATE_RUNNING ) {
		os_ready_add( tcb );
	}
	tcb->state = OS_TASK_STATE_ZOMBIE;
	
	//	Release resources
	mutexArraySize = os_arrayList_size( os_mutex_arraylist_handle );
	for ( uint32_t i = 0; i < mutexArraySize; i++ ) {
		mutex_p = os_arrayList_get( os_mutex_arraylist_handle, i );
		if ( mutex_p->owner == tcb ) {
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
	
	//	Set System to use PSP in thread mode
	__set_CONTROL(0x02);
	__ISB();
		
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
	
	task->state = OS_TASK_STATE_BLOCKED;
	
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



