#include "EOS_Core.h"
#include "EOS_Control.h"

#include <stdio.h>
#include "EOS_Scheduler.h"
#include "EOS_Workers.h"

static os_mutex_t* mutex_array[OS_MAX_MUTEX_COUNT];
static os_arrayList_t mutex_arraylist;
os_arrayList_h os_mutex_arraylist_handle = &mutex_arraylist;



void os_tick( void ) {
	
	uint32_t size = 0;
	
	if ( os_Control.state == OS_STATE_RUNNING ) {
	
		os_Control.tick_counter++;
		
		size = os_get_blocked_task_amount();
		
		//	If there are blocked tasks
		if ( size > 0 ) {
			
			//	Loop through blocked tasks array
			for ( uint32_t i = 0; i < size; i++ ) {
				
				os_TCB_t* task = os_get_blocked_task_by_index( i );

				task->countdown--;
				
				//	If countdown reaches 0, 
				if ( task->countdown == 0 ) {
					os_unblock_task( task );
				}
				
			} 
		}
	}
	
	if ( os_Control.tick_counter >= os_Control.task_switch_tick_count ) {
		printf("Triggering Context Switch\n");
		os_task_switch_trigger();
		os_Control.tick_counter = 0;
	}
}

void os_core_init( uint32_t os_tick_frq ) {
	
	uint32_t* stack = 0;
	
	os_Control.task_switch_tick_count = OS_TASK_SWITCH_TICK_COUNT;
	os_Control.tick_counter = 0;
	
	if ( os_tick_frq == 0 ) {
		uint32_t coreClk = SystemCoreClock;
		uint32_t sysTickResetVal = SysTick->LOAD;
		
		os_tick_frq = coreClk / sysTickResetVal;
	}
	
	os_Control.os_tick_frq = os_tick_frq;
	
	//	Initialize Mutex ArrayList
	os_arrayList_init( os_mutex_arraylist_handle, (void**) mutex_array, OS_MAX_MUTEX_COUNT );
	
	//	Initialize OS Shutdown task
	stack = (uint32_t*) os_exit_worker_H.stack;
	os_arch_create_task( &os_exit_task, (uint32_t*) stack, &os_exit_worker_H.backed_up_registers, 0 );
	
	//	Initialize OS Wait task
	stack = (uint32_t*) os_wait_worker_H.stack;
	os_arch_create_task( &os_wait_task, (uint32_t*) stack, &os_wait_worker_H.backed_up_registers, 0 );
}



