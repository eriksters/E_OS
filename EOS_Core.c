#include "EOS_Control.h"

#include <stdio.h>

static os_tasks_blocked_t os_tasks_blocked;

void os_task_switch_trigger( void ) {
	SCB->ICSR |= 0x1 << 28;
	__DSB();
	__ISB();
} 


__attribute__((weak)) void os_tick_reset( void ) {
	SysTick->VAL = 0;
	
	NVIC_ClearPendingIRQ(SysTick_IRQn);
}


void os_tick( void ) {
	os_Control.tick_counter++;
	
	//	If there are blocked tasks
	if ( os_tasks_blocked.size > 0 ) {
		
		uint32_t tasks_to_check = os_tasks_blocked.size;
		
		//	Loop through blocked tasks array
		for ( uint32_t i = 0; i < os_tasks_blocked.max_size; i++ ) {
			
			os_TCB_t* task = os_tasks_blocked.testArray[i];
			
			if (task != 0) {
				task->countdown--;
				
				//	If countdown reaches 0, 
				if ( task->countdown == 0 ) {
					os_task_blocked_resume( task );
				}
				
				//	If no more blocked tasks to check, break for loop
				tasks_to_check--;
				if (tasks_to_check == 0) {
					break;
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

extern os_TCB_t* os_ready_add( os_TCB_t* E );


void os_task_blocked_resume( os_TCB_t* task ) {
	os_blocked_remove( task );
	os_ready_add( task );
}


void os_core_init( uint32_t os_tick_frq ) {
	
	os_tasks_blocked.size = 0;
	os_tasks_blocked.max_size = 10;
	
	os_Control.task_switch_tick_count = OS_TASK_SWITCH_TICK_COUNT;
	os_Control.tick_counter = 0;
	
	if ( os_tick_frq == 0 ) {
		uint32_t coreClk = SystemCoreClock;
		uint32_t sysTickResetVal = SysTick->LOAD;
		
		os_tick_frq = coreClk / sysTickResetVal;
	}
	
	os_Control.os_tick_frq = os_tick_frq;
}


os_TCB_t* os_blocked_add( os_TCB_t* E ) {
	
	os_TCB_t* ret = 0;
	
	if ( os_tasks_blocked.size >= os_tasks_blocked.max_size ) {
		return ret;
	}
	
	if ( E == 0 ) {
		return ret;
	}
	
	for ( uint32_t i = 0; i < os_tasks_blocked.max_size; i++ ) {
		if ( os_tasks_blocked.testArray[i] == 0 ) {
			os_tasks_blocked.testArray[i] = E;
			os_tasks_blocked.size++;
			ret = E;
			break;
		}
	}
	
	return ret;
}


os_TCB_t* os_blocked_remove( os_TCB_t* E ) {
	
	os_TCB_t* ret = 0;
	
	if ( os_tasks_blocked.size == 0 ) {
		return ret;
	}
	
	if ( E == 0 ) {
		return ret;
	}
	
	for ( uint32_t i = 0; i < os_tasks_blocked.max_size; i++) {
		if ( os_tasks_blocked.testArray[i] == E ) {
			os_tasks_blocked.testArray[i] = 0;
			os_tasks_blocked.size--;
			ret = E;
			break;
		}
	}
	
	return ret;
}
