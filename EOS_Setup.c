#include "EOS_Setup.h"
#include "EOS_Scheduling.h"
#include "EOS_Mutex.h"
#include "EOS_Timing.h"
#include "EOS_Blocking.h"

void os_init_call_handler( uint32_t os_tick_frq ) {
	os_Control.state = OS_STATE_INIT;

	os_core_init();
	os_timing_init( os_tick_frq );
	os_blocking_init();
	os_mutex_init();
	
	os_Control.state = OS_STATE_POST_INIT;
}


void os_start_call_handler( void ) {
	
	os_Control.state = OS_STATE_STARTING;
	
	//	Do not start if no tasks have been created
	if (os_Control.taskCount < 1) {
		return;
	}
	
	//	Set up hardware (stack pointer selection and interrupt priorities)
	os_arch_start();
		
	//	Start the OS
	os_trigger_task_switch();
}

void os_exit_call_handler( void ) {
	os_Control.state = OS_STATE_EXIT;
	os_trigger_task_switch();
}

