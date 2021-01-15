#include "EOS_Timing.h"
#include "EOS_Core.h"
#include "EOS_Scheduling.h"
#include "EOS_Blocking.h"
#include "stm32f10x.h"

static os_task_h os_delayed_task_array[OS_MAX_DELAYED_TASK_COUNT];
static os_arraylist_t os_delayed_task_arraylist;

static os_arraylist_h delayed_tasks_arraylist_H;
static uint32_t os_tick_frq;


void os_timing_init( uint32_t os_tick_frq_ ) {
	
	if ( os_tick_frq_ == 0 ) {
		os_tick_frq_ = os_get_systick_frq();
	}
	
	os_tick_frq = os_tick_frq_;
	
	delayed_tasks_arraylist_H = os_arraylist_init( &os_delayed_task_arraylist, (void**) os_delayed_task_array, OS_MAX_DELAYED_TASK_COUNT );
}

void os_tick( void ) {
	
	if ( os_get_state() == OS_STATE_RUNNING ) {
		uint32_t size = 0;
		
		size = os_arraylist_size( delayed_tasks_arraylist_H );
		
		//	If there are blocked tasks
		if ( size > 0 ) {
			
			//	Loop through blocked tasks array
			for ( uint32_t i = 0; i < size; i++ ) {
				
				os_TCB_t* task = os_arraylist_get( delayed_tasks_arraylist_H, i );
				task->countdown--;
				
				//	If countdown reaches 0, 
				if ( task->countdown == 0 ) {
					os_arraylist_remove( delayed_tasks_arraylist_H, task );
					os_unblock_task( task );
				}
				
			} 
		}
		os_scheduling_tick();
	}
}

void os_delay_call_handler( uint32_t milliseconds ) {
	
	os_task_h task = os_get_current_task();
	
	task->countdown = milliseconds * os_tick_frq / 1000;
	
	os_arraylist_add( delayed_tasks_arraylist_H, task );
	
	os_block_task( task );
	
	os_release_call_handler();
	
}
