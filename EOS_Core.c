#include "EOS_Core.h"
#include "EOS_Control.h"

#include <stdio.h>

static os_tasks_blocked_t os_tasks_blocked;

static os_mutex_t* mutex_array[OS_MAX_MUTEX_COUNT];
static os_arrayList_t mutex_arraylist;
os_arrayList_h os_mutex_arraylist_handle = &mutex_arraylist;

static os_TCB_t* blocked_array[OS_MAX_TASK_COUNT];
static os_arrayList_t blocked_arrayList;
os_arrayList_h os_blocked_arrayList_handle = &blocked_arrayList;

os_TCB_t os_exit_task_handle;
void os_exit_task( void * );

os_TCB_t os_wait_task_handle;
void os_wait_task( void * ) __attribute__((noreturn));




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
	
	uint32_t size = 0;
	
	if ( os_Control.state == OS_STATE_RUNNING ) {
	
		os_Control.tick_counter++;
		
		size = os_blocked_size();
		
		//	If there are blocked tasks
		if ( size > 0 ) {
			
			//	Loop through blocked tasks array
			for ( uint32_t i = 0; i < size; i++ ) {
				
				os_TCB_t* task = os_blocked_get( i );

				task->countdown--;
				
				//	If countdown reaches 0, 
				if ( task->countdown == 0 ) {
					os_task_blocked_resume( task );
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
	task->state = OS_TASK_STATE_READY;
	os_ready_add( task );
}


void os_core_init( uint32_t os_tick_frq ) {
	
	uint32_t* stack = 0;
	
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
	
	//	Initialize Blocked Task ArrayList
	os_arrayList_init( os_blocked_arrayList_handle, (void**) blocked_array, OS_MAX_TASK_COUNT );
	
	//	Initialize Mutex ArrayList
	os_arrayList_init( os_mutex_arraylist_handle, (void**) mutex_array, OS_MAX_MUTEX_COUNT );
	
	//	Initialize OS Shutdown task
	stack = (uint32_t*) os_exit_task_handle.stack;
	os_arch_create_task( &os_exit_task, (uint32_t*) stack, &os_exit_task_handle.backed_up_registers, 0 );
	
	//	Initialize OS Wait task
	stack = (uint32_t*) os_wait_task_handle.stack;
	os_arch_create_task( &os_wait_task, (uint32_t*) stack, &os_wait_task_handle.backed_up_registers, 0 );
}


os_TCB_t* os_blocked_add( os_TCB_t* E ) {
	
	if ( os_arrayList_add( os_blocked_arrayList_handle, E ) ) {
		return 0;
	}
	
	return E;
	
	
	/*
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
	*/
}


os_TCB_t* os_blocked_remove( os_TCB_t* E ) {
	
	if (os_arrayList_remove( os_blocked_arrayList_handle, E )) {
		return 0;
	}
	
	return E;
	
	/*
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
	*/
}


uint32_t os_blocked_size( void ) {
	
	return os_arrayList_size( os_blocked_arrayList_handle );
	
	//	return os_tasks_blocked.size;
}

os_TCB_t* os_blocked_get( uint32_t index ) {
	
	return os_arrayList_get( os_blocked_arrayList_handle, index );

}


extern void os_exit( void );

void os_exit_task( void * param ) {
	UNUSED( param );
	os_exit();
}

void os_wait_task( void * param ) {
	UNUSED( param );
	//__asm("B	.");
	for (;;) {
		printf("OS Wait\n");
	}
}
