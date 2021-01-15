#include "EOS_Core.h"
#include "EOS_Workers.h"
#include "EOS_Blocking.h"
#include "EOS_Mutex.h"

/* Contains state and configuration of the OS.
*/
static os_state_t state;


void os_init_call_handler( uint32_t os_tick_frq ) {
	state = OS_STATE_INIT;
	
	os_scheduling_init();
	os_tasks_init();
	os_timing_init( os_tick_frq );
	os_blocking_init();
	os_mutex_init();
	os_workers_init();
	
	state = OS_STATE_POST_INIT;
}


void os_start_call_handler( void ) {
	
	state = OS_STATE_STARTING;
	
	//	Do not start if no tasks have been created
	if (os_get_task_count() < 1) {
		state = OS_STATE_DONE;
		return;
	}
	
	//	Set up hardware (stack pointer selection and interrupt priorities)
	os_arch_start();
		
	//	Start the OS
	os_trigger_task_switch();
}

void os_exit_call_handler( void ) {
	state = OS_STATE_EXIT;
	os_trigger_task_switch();
}

os_state_t os_get_state( void ) {
	return state;
}

void os_set_state( os_state_t target_state ) {
	state = target_state;
}
