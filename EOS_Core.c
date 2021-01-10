#include "EOS_Core.h"
#include "EOS_Scheduling.h"
#include "EOS_Workers.h"
#include "EOS_Mutex.h"
#include "stm32f10x.h"
#include <stdio.h>


os_Control_t os_Control;


void os_core_init( void ) {
	
	os_Control.currentTask = 0;
	os_Control.taskCount = 0;
	
	uint32_t* stack = 0;
	
	//	Initialize OS Shutdown task
	stack = (uint32_t*) os_exit_worker_H.stack;
	os_arch_create_task( &os_exit_task, (uint32_t*) stack, &os_exit_worker_H.backed_up_registers, 0 );
	os_exit_worker_H.state = OS_TASK_STATE_READY;
	
	//	Initialize OS Wait task
	stack = (uint32_t*) os_wait_worker_H.stack;
	os_arch_create_task( &os_wait_task, (uint32_t*) stack, &os_wait_worker_H.backed_up_registers, 0 );
	os_wait_worker_H.state = OS_TASK_STATE_READY;
}

os_state_t os_get_state( void ) {
	return os_Control.state;
}

void os_set_state( os_state_t state ) {
	os_Control.state = state;
}

void os_set_current_task( os_task_h task ) {
	os_Control.currentTask = task;
}


