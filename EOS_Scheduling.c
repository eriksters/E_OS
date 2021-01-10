#include "EOS_Scheduling.h"
#include "EOS_Blocking.h"
#include "EOS_Workers.h"

static os_task_h os_ready_tasks_array[OS_MAX_TASK_COUNT];
static os_queue_t os_ready_tasks_queue;
static os_queue_h os_ready_tasks_queue_H;

static uint32_t tick_counter;

void os_scheduling_algo_init( void ) {	
	os_ready_tasks_queue_H = os_queue_init( &os_ready_tasks_queue, (void**) os_ready_tasks_array, OS_MAX_TASK_COUNT );	
}

void os_scheduling_tick( void ) {
	
	tick_counter++;
	
	if ( tick_counter >= OS_TASK_SWITCH_TICK_COUNT ) {
		os_trigger_task_switch();
		tick_counter = 0;
	}
}

void os_schedule_task( os_task_h task ) {
	
	//	Only add if queue does not already contain the task
	if ( !os_queue_contains( os_ready_tasks_queue_H, task ) ) {
		os_queue_add( os_ready_tasks_queue_H, task );
		task->state = OS_TASK_STATE_READY;
	}
	
}

void os_deschedule_task( os_task_h task ) {
	
	os_queue_remove_element( os_ready_tasks_queue_H, task );
	
	if ( task->state == OS_TASK_STATE_ZOMBIE ) {
		
		//	Release resources
		os_unlock_all_mutexes_by_task( task );
		
		task->state = OS_TASK_STATE_DELETED;
	}
	
	if ( task == os_get_current_task() ) {
		os_trigger_task_switch();
	}
	
	/*
	os_remove_task_from_blocked( task );
	
	if ( task->state == OS_TASK_STATE_RUNNING ) {
		task->state = OS_TASK_STATE_DELETED;
		os_trigger_task_switch();
	} else {
		task->state = OS_TASK_STATE_ZOMBIE;	
		if ( !os_queue_contains( os_ready_tasks_queue_H, task ) ) {
			os_queue_add( os_ready_tasks_queue_H, task );
		}
	}
	*/
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
		
		previousTask = os_get_current_task();
		
		//	Add previous task to the end of the queue, if it has not been deleted or blocked
		if ( previousTask->state == OS_TASK_STATE_RUNNING ) {
			
			if ( previousTask != &os_wait_worker_H ) {
				os_queue_add( os_ready_tasks_queue_H, previousTask );
			}
			
			previousTask->state = OS_TASK_STATE_READY;
			
			/*
			os_queue_add( os_ready_tasks_queue_H, previousTask );
			previousTask->state = OS_TASK_STATE_READY;
		
		//	If previous Task is in zombie state, change it to deleted
		} else if ( previousTask->state == OS_TASK_STATE_ZOMBIE ) {
			previousTask->state = OS_TASK_STATE_DELETED;
		}
			*/
			
		}
	}
	
	//	Select next task to run
	do {
		nextTask = ( os_task_h ) os_queue_remove( os_ready_tasks_queue_H );
		
		//	If there are no ready tasks...
		if ( nextTask == 0 ) {
			
			//	No blocked tasks: shut down OS
			if ( os_get_blocked_task_amount() == 0 ) {
				os_Control.state = OS_STATE_EXIT;
				nextTask = &os_exit_worker_H;
			
			//	Blocked tasks: run empty code
			} else {
				nextTask = &os_wait_worker_H;
			}
		}
		
			/*
		//	If the task is being deleted, set its state 
		} else if ( nextTask->state == OS_TASK_STATE_ZOMBIE ) {
			nextTask->state = OS_TASK_STATE_DELETED;
		} 
		*/
		
	} while ( nextTask->state != OS_TASK_STATE_READY );
		
	//	Set next task as current task
	os_set_current_task( nextTask );
}

void os_release_call_handler ( void ) {
	os_trigger_task_switch();
}
