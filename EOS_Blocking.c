#include "EOS_Blocking.h"
#include "EOS_Scheduling.h"
#include "EOS_Workers.h"
#include <stdio.h>

static os_task_h os_blocked_tasks_array[OS_MAX_TASK_COUNT];
static os_arraylist_t os_blocked_tasks_arraylist;

static os_arraylist_h os_blocked_tasks_arraylist_H;

void os_blocking_init( void ) {
	os_blocked_tasks_arraylist_H = os_arraylist_init( &os_blocked_tasks_arraylist, (void**) os_blocked_tasks_array, OS_MAX_TASK_COUNT );
	os_scheduling_algo_init();
}


void os_block_task( os_task_h task ) {
	
	//	Task is not in a blockable state
	if ( task->state != OS_TASK_STATE_READY && task->state != OS_TASK_STATE_RUNNING ) {
		return;
	}
	
	os_arraylist_add( os_blocked_tasks_arraylist_H, task );
	
	task->state = OS_TASK_STATE_BLOCKED;
	
	if ( task == os_get_current_task() ) {
		os_trigger_task_switch();
	}
}

void os_unblock_task( os_task_h task ) {
	
	//	Task is not in an unblockable state
	if ( task->state != OS_TASK_STATE_BLOCKED ) {
		return;
	}
	
	os_arraylist_remove( os_blocked_tasks_arraylist_H, task );
	
	//	Add task back to scheduling
	os_schedule_task( task );
}

void os_block_call_handler( void ) {	
	os_block_task( os_get_current_task() );	
}

void os_unblock_call_handler( os_task_h task ) {
	if ( task != 0 ) {
		os_unblock_task( task );
	}
}


uint32_t os_get_blocked_task_amount( void ) {
	return os_arraylist_size( os_blocked_tasks_arraylist_H );
}

uint32_t os_remove_task_from_blocked( os_task_h task ) {
	return os_arraylist_remove( os_blocked_tasks_arraylist_H, task );
}
