#include "EOS_Tasks.h"
#include "EOS_Scheduling.h"
#include "EOS_Mutex.h"
#include <stdio.h>



os_task_h os_create_task_call_handler ( void ( *func )( void * ), os_TCB_t* tcb, void * params ) {
	
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

void os_delete_task_call_handler ( os_task_h task ) {
	
	//	Check if calling task or another task
	if ( task == 0 ) {
		task = os_get_current_task();
	}
	
	//	Task is now a zombie
	task->state = OS_TASK_STATE_ZOMBIE;
	
	//	Schedule for removal
	os_deschedule_task( task );
	
}


void os_task_finished_call_handler ( void ) {
	os_delete_task_call_handler( 0 );
}

os_TCB_t* os_get_current_task( void ) {
	return os_Control.currentTask;
}

void os_set_current_task_state( os_task_state_t state ) {
	os_Control.currentTask->state = state;
}

os_Registers_t* os_get_current_task_reg( void ) {
	return &os_Control.currentTask->backed_up_registers;
}
