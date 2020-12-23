#include "EOS_Core.h"
#include <stdio.h>

static os_tasks_blocked_t os_tasks_blocked;

extern os_Control_t os_Control;

void os_TriggerPendSV( void ) {
	SCB->ICSR |= 0x1 << 28;
	__DSB();
	__ISB();
} 

void os_tick( void ) {
	os_Control.tick_counter++;
	if ( os_Control.tick_counter >= os_Control.task_switch_tick_count ) {
		printf("Triggering Context Switch\n");
		os_TriggerPendSV();
		os_Control.tick_counter = 0;
	
	}
}


void os_init_blocked( void ) {
	os_tasks_blocked.size = 0;
	os_tasks_blocked.max_size = 10;
}

uint32_t* os_add_to_blocked( uint32_t* E ) {
	
	uint32_t* ret = 0;
	
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


uint32_t* os_remove_from_blocked( uint32_t* E ) {
	
	uint32_t* ret = 0;
	
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