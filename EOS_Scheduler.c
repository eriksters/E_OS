#include "EOS_Scheduler.h"
#include "EOS_Control.h"
#include "EOS_Workers.h"
#include <stdio.h>

static os_task_h os_ready_tasks_array[OS_MAX_TASK_COUNT];
static os_queue_t os_ready_tasks_queue;
static os_queue_h os_ready_tasks_queue_H;


static os_task_h os_blocked_tasks_array[OS_MAX_TASK_COUNT];
static os_arraylist_t os_blocked_tasks_arraylist;
static os_arraylist_h os_blocked_tasks_arraylist_H;

void os_scheduler_init( void ) {	
	os_ready_tasks_queue_H = os_queue_init( &os_ready_tasks_queue, (void**) os_ready_tasks_array, OS_MAX_TASK_COUNT );	
	os_blocked_tasks_arraylist_H = os_arraylist_init( &os_blocked_tasks_arraylist, (void**) os_blocked_tasks_array, OS_MAX_TASK_COUNT );
}

/*	Result is impacted by os state:
 *	STARTING: Only picks the next task
 *  RUNNING: Picks the next task and adds previous task back in queue
 *
 *	If previous task has been blocked or is in zombie state, do not add it back in queue.
 *	If next task is blocked or in zombie state, do not set it as current task.
 *  If next task is in zombie state, remove it from scheduling and set as deleted.
*/
void os_switch_current_task( void ) {
	
	os_task_h previousTask = 0;
	os_task_h nextTask = 0;
	
	
	if ( os_Control.state != OS_STATE_STARTING ) {
		
		previousTask = os_ctrl_get_current_task();
		
		//	Add previous task to the end of the queue, if it has not been deleted or blocked
		if ( previousTask->state == OS_TASK_STATE_RUNNING && previousTask != &os_wait_worker_H ) {
			os_queue_add( os_ready_tasks_queue_H, previousTask );		//os_ready_add( previousTask );
			previousTask->state = OS_TASK_STATE_READY;
		
		//	If previous Task is in zombie state, change it to deleted
		} else if ( previousTask->state == OS_TASK_STATE_ZOMBIE ) {
			previousTask->state = OS_TASK_STATE_DELETED;
		}
	}
	
	//	Select next task to run
	do {
		nextTask = ( os_task_h ) os_queue_remove( os_ready_tasks_queue_H );  //os_ready_remove();
		
		//	If there are no ready tasks...
		if ( nextTask == 0 ) {
			
			//	And also no blocked tasks, then shut down OS
			if ( os_get_blocked_task_amount() == 0 ) {
				os_Control.state = OS_STATE_EXIT;
				nextTask = &os_exit_worker_H;
			
			//	But there are blocked tasks, then run empty code
			//	Here low-power setting could be used
			} else {
				nextTask = &os_wait_worker_H;
			}
		}
	} while ( nextTask->state == OS_TASK_STATE_ZOMBIE || nextTask->state == OS_TASK_STATE_BLOCKED );
		
	//	Set next task as current task
	os_Control.currentTask = nextTask;
}


void os_schedule_task( os_task_h task ) {
	
	//	Only add if queue does not already contain the task
	if ( !os_queue_contains( os_ready_tasks_queue_H, task ) ) {
		os_queue_add( os_ready_tasks_queue_H, task );
		task->state = OS_TASK_STATE_READY;
	}
	
}

void os_deschedule_task( os_task_h task ) {
	
	os_arraylist_remove( os_blocked_tasks_arraylist_H, task );
	
	if ( task->state == OS_TASK_STATE_RUNNING ) {
		task->state = OS_TASK_STATE_DELETED;
	} else {
		task->state = OS_TASK_STATE_ZOMBIE;	
		if ( !os_queue_contains( os_ready_tasks_queue_H, task ) ) {
			os_queue_add( os_ready_tasks_queue_H, task );
		}
	}
}


void os_block_task( os_task_h task ) {
	
	//	Task is not in a blockable state
	if ( task->state != OS_TASK_STATE_READY && task->state != OS_TASK_STATE_RUNNING ) {
		return;
	}
	
	os_arraylist_add( os_blocked_tasks_arraylist_H, task );
	
	task->state = OS_TASK_STATE_BLOCKED;
}


void os_unblock_task( os_task_h task ) {
	
	//	Task is not in an unblockable state
	if ( task->state != OS_TASK_STATE_BLOCKED ) {
		return;
	}
	
	os_arraylist_remove( os_blocked_tasks_arraylist_H, task );
	task->state = OS_TASK_STATE_READY;
	
	//	Add task back to scheduling queue
	os_schedule_task( task );
}


uint32_t os_get_blocked_task_amount( void ) {
	return os_arraylist_size( os_blocked_tasks_arraylist_H );
}

os_TCB_t* os_get_blocked_task_by_index( uint32_t index ) {
	return os_arraylist_get( os_blocked_tasks_arraylist_H, index );
}


